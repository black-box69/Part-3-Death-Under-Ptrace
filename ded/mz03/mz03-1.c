enum
{
    BIT_MASK = 0x1
};

//made by: ded

STYPE
bit_reverse(STYPE value)
{
    UTYPE result = 0;
    UTYPE full_bit = ~result;
    UTYPE unsigned_value = (UTYPE) value;
    while (full_bit) {
        result = result << 1;
        result += unsigned_value & BIT_MASK;
        unsigned_value = unsigned_value >> 1;
        full_bit = full_bit >> 1;
    }
    return (STYPE) result;
}
