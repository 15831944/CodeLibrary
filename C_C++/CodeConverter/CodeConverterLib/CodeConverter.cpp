#include "CodeConverter.h"


static unsigned char BOM_UTF16_Little[] = {0xFF, 0xFE};	// Unicode file header
static unsigned char BOM_UTF16_Big[] = {0xFE, 0xFF};	// Unicode big endian file header
static unsigned char BOM_UTF8[] = {0xEF, 0xBB, 0xBF};	// UTF_8 file header

//判断pFirstLine所指字符串是不是为UTF-8无BOM编码格式
//   U-00000000 - U-0000007F: 0xxxxxxx 
//   U-00000080 - U-000007FF: 110xxxxx 10xxxxxx 
//   U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
//   U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx 
//   U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
//   U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
static bool IsUtf8NoBOM(const unsigned char* pFirstLine, const int size)
{
    const unsigned char* p = pFirstLine;

    int mCount = 0;
    int bCount = 0;
    int curCount = 0;
    for(int idx = 0; idx < size; ++idx){
        p = pFirstLine + idx;
        const unsigned int val = *p;
        //0xxxxxxx
        if((*p & 0x80) == 0x0){
            bCount = 0;
        //10xxxxxx
        }else if((val & 0xC0) == 0x80){
            if(bCount > 0){
                curCount--;

                if(curCount == 0){
                    mCount++;
                }
            }
        //110xxxxx
        }else if((val & 0xE0) == 0xC0){
            bCount = 2;
            curCount = 1;
        //1110xxxx
        }else if((val & 0xF0) == 0xE0){
            bCount = 3;
            curCount = 2;
        //11110xxx
        }else if((val & 0xF8) == 0xF0){
            bCount = 4;
            curCount = 3;
        //111110xx
        }else if((val & 0xFC) == 0xF8){
            bCount = 5;
            curCount = 4;
        //1111110x
        }else if((val & 0xFE) == 0xFC){
            bCount = 6;
            curCount = 5;
        }
    }

    if(mCount > 0){
        return true;
    }else{
        return false;
    }
}


//判断pFirstLine所指字符串是不是为GB18030/GB2321格式
//单字节: 0到0x7F
//      0000.0000-0111.1111
//双字节: 第一个字节的值从0x81到0xFE，第二个字节的值从0x40到0xFE(不包括0x7F)
//      1000.0001-1111.1110  0100.0000-1111.1110
//四字节，第一个字节的值从0x81到0xFE，第二个字节的值从0x30到0x39，第三个字节从0x81到0xFE，第四个字节从0x30到0x39
//      1000.0001-1111.1110  0011.0000-0011.10001  1000.0001-1111.1110  0011.0000-0011.10001
static int IsGB18030(const unsigned char* pFirstLine, const int size)
{
    const unsigned char* p = pFirstLine;

    int preBit = 1;
    int curBit = 0;
    int idxBit = 1;

    int matchCount = 0;
    int missCount = 0;

    bool isM2 = false; 
    bool isM4 = false;
    for(int idx = 0; idx < size; ++idx){
        p = pFirstLine + idx;
        const unsigned int val = *p;

        if(val == 0xFF){
            missCount++;
            idxBit = 1;
            isM2 = false;
            isM4 = false;
        }

        if(idxBit == 1){
            if((val & 0x8F) == 0x0){
                //matchCount++;
                isM2 = false;
                isM4 = false;
            }else if(val >= 0x81 && val <= 0xFE){
                idxBit = 2;
                isM2 = true;
                isM4 = true;
            }else{
                missCount++;
                idxBit = 1;
                isM2 = false;
                isM4 = false;
            }
        }else if(idxBit == 2){
            if((val & 0xC0) != 0x0){
                matchCount++;
                idxBit = 1;
                isM2 = false;
                isM4 = false;
            }else if(val >= 0x30 && val <= 0x39){
                idxBit = 3;
                isM2 = false;
                isM4 = true;
            }else{
                missCount++;
                idxBit = 1;
                isM2 = false;
                isM4 = false;
            }
        }else if(idxBit == 3){
            if(val >= 0x81 && val <= 0xFE){
                idxBit = 4;
                isM2 = true;
                isM4 = true;
            }else{
                missCount++;
                idxBit = 1;
                isM2 = false;
                isM4 = false;
            }
        }else if(idxBit == 4){
            if(val >= 0x30 && val <= 0x39){
                matchCount++;
                idxBit = 3;
                isM2 = false;
                isM4 = true;
            }else{
                missCount++;
                idxBit = 1;
                isM2 = false;
                isM4 = false;
            }
        }
    }

    if(matchCount > 0 && missCount == 0){
        return true;
    }else{
        return false;
    }
}

CodeType CCodeConverter::GetCodeType(const unsigned char* pLine, const int size, const bool isFirstLine){
    CodeType res;
    const unsigned char* p = pLine;
    if(size < 2){
        res = CT_NONE;
    }else{
        if(isFirstLine){
            if(p[0] == BOM_UTF16_Little[0] && p[1] == BOM_UTF16_Little[1]){
                res = CT_UTF16_L;
            }else if(p[0] == BOM_UTF16_Big[0] && p[1] == BOM_UTF16_Big[1]){
                res = CT_UTF16_B;
            }else if (size >= 3 && p[0] == BOM_UTF8[0] && p[1] == BOM_UTF8[1] && p[2] == BOM_UTF8[2]){
                res = CT_UTF8_BOM;
            }else{
                bool bRes = IsUtf8NoBOM(pLine, size);
                if(bRes){
                    res = CT_UTF8_NO_BOM;
                }else if(bRes = IsGB18030(pLine, size)){
                    res = CT_GB18030;
                }else{
                    res = CT_NONE;
                }
            }
        }else{
            bool bRes = IsUtf8NoBOM(pLine, size);
            if(bRes){
                res = CT_UTF8_NO_BOM;
            }else if(bRes = IsGB18030(pLine, size)){
                res = CT_GB18030;
            }else{
                res = CT_NONE;
            }
        }
    }

    return res;
}


//获取指定文件的编码格式
CodeType CCodeConverter::GetCodeType(ifstream& pFile)
{
    const int LINE_LENGTH = 1024 * 5;
    char strLine[LINE_LENGTH];

    int line = 0;
    CodeType cType;

    while(!pFile.eof()){
        pFile.getline(strLine, LINE_LENGTH);
        line++;

        if(line == 1){
            cType = CCodeConverter::GetCodeType((unsigned char*)strLine, strlen(strLine), true);
        }else{
            cType = CCodeConverter::GetCodeType((unsigned char*)strLine, strlen(strLine), false);
        }

        if(cType != CT_NONE){
            break;
        }
    }

    return cType;
}


/* -------------------------------------------------------------
内码转换
------------------------------------------------------------- */

// 转换UCS4编码到UTF8编码
INT CCodeConverter::UCS4_To_UTF8( DWORD dwUCS4, BYTE* pbUTF8 )
{
    const BYTE	abPrefix[] = {0, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};
    const DWORD adwCodeUp[] = {
        0x80,			// U+00000000 ～ U+0000007F
        0x800,			// U+00000080 ～ U+000007FF
        0x10000,		// U+00000800 ～ U+0000FFFF
        0x200000,		// U+00010000 ～ U+001FFFFF
        0x4000000,		// U+00200000 ～ U+03FFFFFF
        0x80000000		// U+04000000 ～ U+7FFFFFFF
    };

    INT	i, iLen;

    // 根据UCS4编码范围确定对应的UTF-8编码字节数
    iLen = sizeof(adwCodeUp) / sizeof(DWORD);
    for( i = 0; i < iLen; i++ )
    {
        if( dwUCS4 < adwCodeUp[i] )
        {
            break;
        }
    }

    if( i == iLen )return 0;	// 无效的UCS4编码

    iLen = i + 1;	// UTF-8编码字节数
    if( pbUTF8 != NULL )
    {	// 转换为UTF-8编码
        for( ; i > 0; i-- )
        {
            pbUTF8[i] = static_cast<BYTE>((dwUCS4 & 0x3F) | 0x80);
            dwUCS4 >>= 6;
        }

        pbUTF8[0] = static_cast<BYTE>(dwUCS4 | abPrefix[iLen - 1]);
    }

    return iLen;
}

// 转换UTF8编码到UCS4编码
INT CCodeConverter::UTF8_To_UCS4( const BYTE* pbUTF8, DWORD& dwUCS4 )
{
    INT		i, iLen;
    BYTE	b;

    if( pbUTF8 == NULL )
    {	// 参数错误
        return 0;
    }

    b = *pbUTF8++;
    if( b < 0x80 )
    {
        dwUCS4 = b;
        return 1;
    }

    if( b < 0xC0 || b > 0xFD )
    {	// 非法UTF8
        return 0; 
    }

    if( b < 0xE0 )
    {
        dwUCS4 = b & 0x1F;
        iLen = 2;
    }
    else if( b < 0xF0 )
    {
        dwUCS4 = b & 0x0F;
        iLen = 3;
    }
    else if( b < 0xF8 )
    {
        dwUCS4 = b & 7;
        iLen = 4;
    }
    else if( b < 0xFC )
    {
        dwUCS4 = b & 3;
        iLen = 5;
    }
    else
    {
        dwUCS4 = b & 1;
        iLen = 6;
    }

    for( i = 1; i < iLen; i++ )
    {
        b = *pbUTF8++;
        if( b < 0x80 || b > 0xBF )
        {	// 非法UTF8
            break;
        }

        dwUCS4 = (dwUCS4 << 6) + (b & 0x3F);
    }

    if( i < iLen )
    {	// 非法UTF8
        return 0;
    }
    else
    {
        return iLen;
    }
}

// 转换UCS4编码到UCS2编码
INT CCodeConverter::UCS4_To_UTF16( DWORD dwUCS4, WORD* pwUTF16 )
{
    if( dwUCS4 <= 0xFFFF )
    {
        if( pwUTF16 != NULL )
        {
            *pwUTF16 = static_cast<WORD>(dwUCS4);
        }

        return 1;
    }
    else if( dwUCS4 <= 0xEFFFF )
    {
        if( pwUTF16 != NULL )
        {
            pwUTF16[0] = static_cast<WORD>( 0xD800 + (dwUCS4 >> 10) - 0x40 );	// 高10位
            pwUTF16[1] = static_cast<WORD>( 0xDC00 + (dwUCS4 & 0x03FF) );		// 低10位
        }

        return 2;
    }
    else
    {
        return 0;
    }
}

// 转换UCS2编码到UCS4编码
INT CCodeConverter::UTF16_To_UCS4( const WORD* pwUTF16, DWORD& dwUCS4 )
{
    WORD	w1, w2;

    if( pwUTF16 == NULL )
    {	// 参数错误
        return 0;
    }

    w1 = pwUTF16[0];
    if( w1 >= 0xD800 && w1 <= 0xDFFF )
    {	// 编码在替代区域（Surrogate Area）
        if( w1 < 0xDC00 )
        {
            w2 = pwUTF16[1];
            if( w2 >= 0xDC00 && w2 <= 0xDFFF )
            {
                dwUCS4 = (w2 & 0x03FF) + (((w1 & 0x03FF) + 0x40) << 10);
                return 2;
            }
        }

        return 0;	// 非法UTF16编码	
    }
    else
    {
        dwUCS4 = w1;
        return 1;
    }
}

// 转换UTF8字符串到UTF16字符串
INT CCodeConverter::UTF8Str_To_UTF16Str( const BYTE* pbszUTF8Str, WORD* pwszUTF16Str )
{
    INT		iNum, iLen;
    DWORD	dwUCS4;

    if( pbszUTF8Str == NULL )
    {	// 参数错误
        return 0;
    }

    iNum = 0;	// 统计有效字符个数
    while( *pbszUTF8Str )
    {	// UTF8编码转换为UCS4编码
        iLen = UTF8_To_UCS4( pbszUTF8Str, dwUCS4 );
        if( iLen == 0 )
        {	// 非法的UTF8编码
            return 0;
        }

        pbszUTF8Str += iLen;

        // UCS4编码转换为UTF16编码
        iLen = UCS4_To_UTF16( dwUCS4, pwszUTF16Str );
        if( iLen == 0 )
        {
            return 0;
        }

        if( pwszUTF16Str != NULL )
        {
            pwszUTF16Str += iLen;
        }

        iNum += iLen;
    }

    if( pwszUTF16Str != NULL )
    {
        *pwszUTF16Str = 0;	// 写入字符串结束标记
    }

    return iNum;
}

// 转换UTF16字符串到UTF8字符串
INT CCodeConverter::UTF16Str_To_UTF8Str( const WORD* pwszUTF16Str, BYTE* pbszUTF8Str )
{
    INT		iNum, iLen;
    DWORD	dwUCS4;

    if( pwszUTF16Str == NULL )
    {	// 参数错误
        return 0;
    }

    iNum = 0;
    while( *pwszUTF16Str )
    {	// UTF16编码转换为UCS4编码
        iLen = UTF16_To_UCS4( pwszUTF16Str, dwUCS4 );
        if( iLen == 0 )
        {	// 非法的UTF16编码
            return 0;	
        }

        pwszUTF16Str += iLen;

        // UCS4编码转换为UTF8编码
        iLen = UCS4_To_UTF8( dwUCS4, pbszUTF8Str );
        if( iLen == 0 )
        {
            return 0;
        }

        if( pbszUTF8Str != NULL )
        {
            pbszUTF8Str += iLen;
        }

        iNum += iLen;
    }

    if( pbszUTF8Str != NULL )
    {
        *pbszUTF8Str = 0;	// 写入字符串结束标记
    }

    return iNum;
}

/* -------------------------------------------------------------
C文件写入操作
------------------------------------------------------------- */

// 向文件中输出UTF8编码
UINT CCodeConverter::Print_UTF8_By_UCS4( FILE* out, DWORD dwUCS4 )
{
    INT		iLen;
    BYTE	abUTF8[8];

    if( out == NULL )
    {
        return 0;
    }

    iLen = UCS4_To_UTF8( dwUCS4, abUTF8 );
    if( iLen == 0 )return 0;

    fwrite( abUTF8, 1, iLen, out );

    return iLen;
}

// 向文件中输出UTF16编码
UINT CCodeConverter::Print_UTF16_By_UCS4( FILE* out, DWORD dwUCS4, BOOL isBigEndian )
{
    INT		i, iLen;
    WORD	wCode, awUTF16[2];

    if( out == NULL )
    {
        return 0;
    }

    iLen = UCS4_To_UTF16( dwUCS4, awUTF16 );
    if( iLen == 0 )return 0;

    for( i = 0; i < iLen; i++ )
    {
        wCode = awUTF16[i];
        if( isBigEndian )
        {
            fputc( wCode >> 8, out );	// 输出高位
            fputc( wCode & 0xFF, out );	// 输出低位
        }
        else
        {
            fputc( wCode & 0xFF, out );	// 输出低位
            fputc( wCode >> 8, out );	// 输出高位
        }
    }

    return (iLen << 1);
}

// 将UTF16字符串以UTF8编码输出到文件中
UINT CCodeConverter::Print_UTF8Str_By_UTF16Str( FILE* out, const WORD* pwszUTF16Str )
{
    INT		iCount, iLen;
    DWORD	dwUCS4;

    if( (out == NULL) || (pwszUTF16Str == NULL) )
    {
        return 0;
    }

    iCount = 0;
    while( *pwszUTF16Str )
    {	// 将UTF16编码转换成UCS4编码
        iLen = UTF16_To_UCS4( pwszUTF16Str, dwUCS4 );
        if( iLen == 0 )
        {
            break;
        }

        pwszUTF16Str += iLen;

        // 向文件中输出UTF8编码
        iCount += Print_UTF8_By_UCS4( out, dwUCS4 );
    }

    return iCount;	// 输出的字节数
}

// 将UTF8字符串以UTF16编码输出到文件中
UINT CCodeConverter::Print_UTF16Str_By_UTF8Str( FILE* out, const BYTE* pbszUTF8Str, BOOL isBigEndian )
{
    INT		iCount, iLen;
    DWORD	dwUCS4;

    if( (out == NULL) || (pbszUTF8Str == NULL) )
    {
        return 0;
    }

    iCount = 0;
    while( *pbszUTF8Str )
    {	// 将UTF16编码转换成UCS4编码
        iLen = UTF8_To_UCS4( pbszUTF8Str, dwUCS4 );
        if( iLen == 0 )
        {
            break;
        }

        pbszUTF8Str += iLen;

        // 向文件中输出UTF8编码
        iCount += Print_UTF16_By_UCS4( out, dwUCS4, isBigEndian );
    }

    return iCount;	// 输出的字节数
}

// 向文件中输出UTF8字节序标记
UINT CCodeConverter::Print_UTF8_BOM( FILE* out )
{
    if( out == NULL )
    {
        return 0;
    }

    fputc( 0xEF, out );
    fputc( 0xBB, out );
    fputc( 0xBF, out );

    return 3;
}

// 向文件中输出UTF16字节序标记
UINT CCodeConverter::Print_UTF16_BOM( FILE* out, BOOL isBigEndian )
{
    if( out == NULL )
    {
        return 0;
    }

    if( isBigEndian )
    {
        fputc( 0xFE, out );
        fputc( 0xFF, out );
    }
    else
    {
        fputc( 0xFF, out );
        fputc( 0xFE, out );
    }

    return 2;
}

/* -------------------------------------------------------------
C++流输出操作
------------------------------------------------------------- */

// 向流中输出UTF8编码
UINT CCodeConverter::Print_UTF8_By_UCS4( ostream& os, DWORD dwUCS4 )
{
    INT		iLen;
    BYTE	abUTF8[8];

    if( !os )return 0;

    iLen = UCS4_To_UTF8( dwUCS4, abUTF8 );
    if( iLen == 0 )return 0;

    os.write( reinterpret_cast<CHAR*>(abUTF8), iLen );

    return iLen;	
}

// 向流中输出UTF16编码
UINT CCodeConverter::Print_UTF16_By_UCS4( ostream& os, DWORD dwUCS4, BOOL isBigEndian )
{
    INT		i, iLen;
    WORD	wCode, awUTF16[2];

    if( !os )return 0;

    iLen = UCS4_To_UTF16( dwUCS4, awUTF16 );
    if( iLen == 0 )return 0;

    for( i = 0; i < iLen; i++ )
    {
        wCode = awUTF16[i];
        if( isBigEndian )
        {
            os.put( wCode >> 8 );		// 输出高位
            os.put( wCode & 0xFF );		// 输出低位
        }
        else
        {
            os.put( wCode & 0xFF );		// 输出低位
            os.put( wCode >> 8 );		// 输出高位
        }
    }

    return (iLen << 1);
}

// 将UTF16字符串以UTF8编码输出到流中
UINT CCodeConverter::Print_UTF8Str_By_UTF16Str( ostream& os, const WORD* pwszUTF16Str )
{
    INT		iCount, iLen;
    DWORD	dwUCS4;

    if( !os || (pwszUTF16Str == NULL) )return 0;

    iCount = 0;
    while( *pwszUTF16Str )
    {	// 将UTF16编码转换成UCS4编码
        iLen = UTF16_To_UCS4( pwszUTF16Str, dwUCS4 );
        if( iLen == 0 )
        {
            break;
        }

        pwszUTF16Str += iLen;

        // 向流中输出UTF8编码
        iCount += Print_UTF8_By_UCS4( os, dwUCS4 );
    }

    return iCount;	// 输出的字节数
}

// 将UTF8字符串以UTF16编码输出到流中
UINT CCodeConverter::Print_UTF16Str_By_UTF8Str( ostream& os, const BYTE* pbszUTF8Str, BOOL isBigEndian )
{
    INT		iCount, iLen;
    DWORD	dwUCS4;

    if( !os || (pbszUTF8Str == NULL) )return 0;

    iCount = 0;
    while( *pbszUTF8Str )
    {	// 将UTF16编码转换成UCS4编码
        iLen = UTF8_To_UCS4( pbszUTF8Str, dwUCS4 );
        if( iLen == 0 )
        {
            break;
        }

        pbszUTF8Str += iLen;

        // 向流中输出UTF8编码
        iCount += Print_UTF16_By_UCS4( os, dwUCS4, isBigEndian );
    }

    return iCount;	// 输出的字节数
}

// 向流中输出UTF8字节序标记
UINT CCodeConverter::Print_UTF8_BOM( ostream& os )
{
    if( !os )return 0;

    os.put( 0xEF );
    os.put( 0xBB );
    os.put( 0xBF );

    return 3;	
}

// 向流中输出UTF16字节序标记
UINT CCodeConverter::Print_UTF16_BOM( ostream& os, BOOL isBigEndian )
{
    if( !os )return 0;

    if( isBigEndian )
    {
        os.put( 0xFE );
        os.put( 0xFF );
    }
    else
    {
        os.put( 0xFF );
        os.put( 0xFE );
    }

    return 2;
}

/* ------------------------------
END
------------------------------ */
