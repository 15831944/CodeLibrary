local  PrecisionEqual = 0.000001;


function NumberEqual(n1, n2)
    if math.abs(n1 - n2) < PrecisionEqual then
        return true;
    else
        return false;
    end
end


local function TestNumberEqual()
    print("[TestNumberEqual]start.");
    
    local n1, n2;
    
    n1 = 1.01;
    n2 = 1.02;
    if NumberEqual(n1, n2) then
        print("[Error]n1=" .. n1 .. ", n2=" .. n2);
    end
    
    n1 = 1.001;
    n2 = 1.002;
    if NumberEqual(n1, n2) then
        print("[Error]n1=" .. n1 .. ", n2=" .. n2);
    end
    
    n1 = 1.0001;
    n2 = 1.0002;
    if NumberEqual(n1, n2) then
        print("[Error]n1=" .. n1 .. ", n2=" .. n2);
    end
    
    
    n1 = 1.00001;
    n2 = 1.00002;
    if NumberEqual(n1, n2) then
        print("[Error]n1=" .. n1 .. ", n2=" .. n2);
    end
    
    n1 = 1.00003;
    n2 = 1.00002;
    if NumberEqual(n1, n2) then
        print("[Error]n1=" .. n1 .. ", n2=" .. n2);
    end
    
    n1 = 1.00002;
    n2 = 1.00002;
    if not NumberEqual(n1, n2) then
        print("[Error]n1=" .. n1 .. ", n2=" .. n2);
    end
    
    print("[TestNumberEqual]end.");
end


TestNumberEqual();