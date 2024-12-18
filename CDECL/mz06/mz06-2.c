//made by: CDECL

void
normalize_path(char *buf)
{
    if (!buf) {
        return;
    }
    int i = 0, j = 0, n = 0;
    while (buf[n] != '\0') {
        n++;
    }
    while (i < n) {
        if (buf[i] == '/') {
            if (i + 3 <= n && buf[i + 1] == '.' && buf[i + 2] == '.' && (buf[i + 3] == '/' || buf[i + 3] == '\0')) {
                if (j != 0) {
                    j--;
                    while (j > 0 && buf[j] != '/') {
                        j--;
                    }
                }
                i += 3;
            } else if (i + 2 <= n && buf[i + 1] == '.' && (buf[i + 2] == '/' || buf[i + 2] == '\0')) {
                i += 2;
            } else if (i + 1 <= n && (buf[i + 1] == '/' || buf[i + 1] == '\0')) {
                i++;
            } else {
                buf[j] = '/';
                i++;
                j++;
            }
        } else {
            buf[j] = buf[i];
            i++;
            j++;
        }
    }
    if (j == 0) {
        buf[j + 1] = '\0';
    } else {
        buf[j] = '\0';
    }
}

