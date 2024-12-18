enum
{
    MY_INT_MIN = (int) ~(((unsigned) ~0) >> !0)
};

//made by: CDECL

enum
{
    MY_INT_MAX = (int) (((unsigned) ~0) >> !0)
};

int
satsum(int v1, int v2)
{
    if (v1 > 0 && v2 > 0 && v1 > MY_INT_MAX - v2) {
        return MY_INT_MAX;
    } else if (v1 < 0 && v2 < 0 && v1 < MY_INT_MIN - v2) {
        return MY_INT_MIN;
    }
    return v1 + v2;
}

