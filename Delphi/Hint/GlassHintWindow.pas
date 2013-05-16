{========================================================================
 玻璃效果的Hint实现
 Auth: ganlong
 Build Time: 2012-07-04
 ========================================================================}

unit GlassHintWindow;

interface

uses
  Windows, Classes, Controls, Graphics, Forms, SysUtils, ExtCtrls;


type
  TGlassHintWindow = class(THintWindow)
  private
    FWndBmp: TBitmap;   //窗口位图
    FHintBmp: TBitmap;  //提示信息位图

  protected
    procedure CreateParams(var Params: TCreateParams); override;
    procedure Paint; override;
    procedure NCPaint(DC: HDC); override;

    {画提示的图象}
    procedure FullHintImg(Bmp:TBitmap);
    procedure DrawHintFont(Bmp:TBitmap; AHint: string);
    procedure TranslucentHintImg(desBmp, srcBmp: TBitmap; desRect, srcRect: TRect; alphaValue: Integer; transColor: TColor);

    {取得提示窗口对应的桌面区域的图象}
    procedure GetDesktopImg(Bmp: TBitmap; R: TRect);
    {对桌面区域图象作处理,使其看起来像一块玻璃且带有一点阴影}
    procedure EffectHandle(WndBmp, HintBmp: TBitmap; const AHint: string);
  public
    constructor Create(Aowner: TComponent); override;
    destructor Destroy; override;
    procedure ActivateHint(Rect: TRect; const AHint: string); override;
  end;

implementation

const
  GLASS_WIDTH = 2;              //玻璃边块的宽度
  GLASS_HINT_SPACE = 4;         //文本内容的外边距
  GLASS_SHADE_WIDTH = 2;        //阴影的宽度

{ TGlassHintWindow }

procedure TGlassHintWindow.Paint;
begin
  Canvas.CopyRect(ClientRect, FWndBmp.Canvas, ClientRect);
end;


procedure TGlassHintWindow.ActivateHint(Rect: TRect; const AHint: string);
var
  P: TPoint;
begin
  //在这里取得一个适当的尺寸显示文字
  FHintBmp.Width := FHintBmp.Canvas.TextWidth(AHint) + 10;
  FHintBmp.Height := FHintBmp.Canvas.TextHeight(AHint) + GLASS_HINT_SPACE * 2;

  FWndBmp.Width := FHintBmp.Width + GLASS_WIDTH * 2 + GLASS_SHADE_WIDTH;
  FWndBmp.Height := FHintBmp.Height + GLASS_WIDTH * 2 + GLASS_SHADE_WIDTH;

  Rect.Right := Rect.Left + FWndBmp.Width;
  Rect.Bottom := Rect.Top + FWndBmp.Height;

  BoundsRect := Rect;

  if Left < Screen.DesktopLeft then  Left := Screen.DesktopLeft;
  if Top < Screen.DesktopTop then Top := Screen.DesktopTop;
  if Left + Width > Screen.DesktopWidth then Left := Screen.DesktopWidth - Width;
  if Top + Height > Screen.DesktopHeight then Top := Screen.DesktopHeight - Height;

  FullHintImg(FHintBmp);
  GetDesktopImg(FWndBmp, BoundsRect);

  EffectHandle(FWndBmp, FHintBmp, AHint);

  P := ClientToScreen(Point(0, 0));
  SetWindowPos(Handle, HWND_TOPMOST, P.X, P.Y, 0, 0,
      SWP_SHOWWINDOW or SWP_NOACTIVATE or SWP_NOSIZE);

  Rect.Left := Rect.Left + 100;
end;


constructor TGlassHintWindow.Create(Aowner: TComponent);
begin
  inherited;
  FWndBmp := TBitmap.Create;
  FWndBmp.PixelFormat := pf24bit;
  FHintBmp := TBitmap.Create;
end;


procedure TGlassHintWindow.CreateParams(var Params: TCreateParams);

begin
  inherited;
  //去掉窗口边框
  Params.Style := Params.Style and not WS_BORDER;
end;


destructor TGlassHintWindow.Destroy;
begin
  FWndBmp.Free;
  FHintBmp.Free;

  inherited;
end;


procedure TGlassHintWindow.GetDesktopImg(Bmp: TBitmap; R: TRect);
var
  C: TCanvas;
begin
  C:= TCanvas.Create;

  try
    C.Handle := GetDC(0);
    Bmp.Canvas.CopyRect(Rect(0, 0, Bmp.Width, Bmp.Height), C, R);
  finally
    C.Free;
  end;
end;


procedure TGlassHintWindow.EffectHandle(WndBmp, HintBmp: TBitmap; const AHint: string);
var
  R, RText: TRect;
  i, j: Integer;
  P: PByteArray;
  Transt, TranstAngle: Integer;
begin
  R := Rect(0, 0, WndBmp.Width - GLASS_SHADE_WIDTH, WndBmp.Height - GLASS_SHADE_WIDTH);
  Frame3D(WndBmp.Canvas, R, clMedGray, clBtnShadow, 1);

  //作窗口底下的阴影效果
  Transt := 60;
  for j:= WndBmp.Height - GLASS_SHADE_WIDTH - 1 to WndBmp.Height - 1 do
  begin
    P := WndBmp.ScanLine[j];
    TranstAngle := Transt;
    for i:= GLASS_SHADE_WIDTH to WndBmp.Width - 1 do
    begin
      //如果正处于右下角
      if i > WndBmp.Width - 2*GLASS_SHADE_WIDTH  then
      begin
        P[3*i] := P[3*i] * TranstAngle div 100;
        P[3*i + 1] := P[3*i + 1] * TranstAngle div 100;
        P[3*i + 2] := P[3*i + 2] * TranstAngle div 100;
        TranstAngle := TranstAngle + 10;

        if TranstAngle > 90 then TranstAngle := 90;
      end
      else begin
        P[3*i] := P[3*i] * Transt div 100;
        P[3*i + 1] := P[3*i + 1] * Transt div 100;
        P[3*i + 2] := P[3*i + 2] * Transt div 100;
      end;
    end;
    Transt := Transt + 10;
  end;

  //作窗口右边的阴影效果
  for j := GLASS_SHADE_WIDTH to WndBmp.Height - GLASS_SHADE_WIDTH - 1 do
  begin
    P := WndBmp.ScanLine[j];
    Transt := 60;
    for i:= WndBmp.Width - GLASS_SHADE_WIDTH - 1 to WndBmp.Width -1 do
    begin
      P[3*i] := P[3*i] * Transt div 100;
      P[3*i + 1] := P[3*i + 1] * Transt div 100;
      P[3*i + 2] := P[3*i + 2] * Transt div 100;
      Transt := Transt + 10;
    end;
  end;


  RText := Rect(0, 0, HintBmp.Width, HintBmp.Height);
  R := Rect(GLASS_WIDTH, GLASS_WIDTH, HintBmp.Width + GLASS_WIDTH, HintBmp.Height + GLASS_WIDTH);
  TranslucentHintImg(HintBmp, WndBmp, RText, R, 250, HintBmp.Canvas.Font.Color);

  DrawHintFont(HintBmp, AHint);

  WndBmp.Canvas.Draw(GLASS_WIDTH, GLASS_WIDTH, HintBmp);
end;


procedure TGlassHintWindow.NCPaint;
begin
  //重载不让画边框
end;

procedure TGlassHintWindow.FullHintImg(Bmp: TBitmap);
begin
  Bmp.Canvas.Brush.Color := Application.HintColor;
  Bmp.Canvas.Pen.Color := Application.HintColor;
  Bmp.Canvas.Rectangle(0, 0, Bmp.Width, Bmp.Height);
  Bmp.Canvas.Font.Color := Screen.HintFont.Color;
end;

procedure TGlassHintWindow.DrawHintFont(Bmp: TBitmap; AHint: string);
var
  R: TRect;
  iPos: Integer;
begin
  R := Rect(4, 0, Bmp.Width, Bmp.Canvas.TextHeight(AHint) );
  iPos := (Bmp.Height - Bmp.Canvas.TextHeight(AHint) ) div 2;
  R.Top := R.Top + iPos;
  R.Bottom := R.Bottom  + iPos;

  DrawText(Bmp.Canvas.Handle, PChar(AHint), -1, R,
      DT_LEFT or DT_VCENTER	 or DT_NOPREFIX or DT_WORDBREAK or DrawTextBiDiModeFlagsReadingOnly);
end;


procedure TGlassHintWindow.TranslucentHintImg(desBmp, srcBmp: TBitmap; desRect, srcRect: TRect; alphaValue: Integer; transColor: TColor);
var
  rWidth, rHeight, x, y, desIndex, srcIndex: Integer;
  r, rSrc, rDes, g, gSrc, gDes, b, bSrc, bDes:Integer;
  desColor, srcColor:TColor;
begin
  rWidth := desRect.Right - desRect.Left;
  rHeight := desRect.Bottom - desRect.Top;
  if (rWidth <> srcRect.Right - srcRect.Left) or (rHeight <> srcRect.Bottom - srcRect.Top) then
  begin
    Exit;
  end;

  for x := 0 to rWidth - 1 do
  begin
    for y := 0 to rHeight - 1 do
    begin
      desColor := desBmp.Canvas.Pixels[desRect.Left + x, desRect.Top + y];

      if (transColor <> 0) and (transColor = desColor) then
      begin
        Continue;
      end;

      rDes := GetRValue(ColorToRGB(desColor));
      gDes := GetGValue(ColorToRGB(desColor));
      bDes := GetBValue(ColorToRGB(desColor));

      srcColor := srcBmp.Canvas.Pixels[srcRect.Left + x, srcRect.Top + y];
      rSrc := GetRValue(ColorToRGB(srcColor));
      gSrc := GetGValue(ColorToRGB(srcColor));
      bSrc := GetBValue(ColorToRGB(srcColor));

      r := ((alphaValue*rDes)+((255-alphaValue)*rSrc)) div 256;
      g := ((alphaValue*gDes)+((255-alphaValue)*gSrc)) div 256;
      b := ((alphaValue*bDes)+((255-alphaValue)*bSrc)) div 256;

      desBmp.Canvas.Pixels[x+desRect.Left, y+desRect.Top]:=RGB(R,G,B);
    end;
  end;
end;


initialization
  Application.ShowHint := False;
  HintWindowClass := TGlassHintWindow;
  Application.ShowHint := True;


end.
