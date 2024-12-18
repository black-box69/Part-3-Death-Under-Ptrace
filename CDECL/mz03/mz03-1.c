//made by: CDECL

STYPE
bit_reverse(STYPE value)
{
    UTYPE mark = ~0;
    UTYPE ans = 0;
    UTYPE start = value;
    while (mark > 0) {
        ans <<= 1;
        ans ^= start & 1;
        start >>= 1;
        mark >>= 1;
    }
    return (STYPE) ans;
}

