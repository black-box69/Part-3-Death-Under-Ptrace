enum
{
    MY_INT_MAX = (signed) ((unsigned) (~0) >> !0), // максимальное значение знакового 32-битного типа
    MY_INT_MIN = (signed) (~(unsigned) (MY_INT_MAX)) // минимальное значение знакового 32-битного типа
};

//made by: ded

int
satsum(int v1, int v2)
{
    if (v1 > 0 && v2 > 0) {
        if (MY_INT_MAX - v1 < v2) {
            return MY_INT_MAX;
        }
    }
    if (v1 < 0 && v2 < 0) {
        if (MY_INT_MIN - v1 > v2) {
            return MY_INT_MIN;
        }
    }
    return v1 + v2;
}
