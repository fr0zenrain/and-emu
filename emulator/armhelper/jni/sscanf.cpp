#include "base.h"

#define ISSPACE " \t\n\r\f\v"

static int s_strcspn (const char *p, const char *s)
{
    int i, j;

    for (i = 0; p[i]; i++) {
        for (j = 0; s[j]; j++) {
            if (s[j] == p[i])
                break;
        }
        if (s[j])
            break;
    }
    return (i);
}


static char *_getbase(char *p, int *basep)
{
    if (p[0] == '0') {
        switch (p[1]) {
        case 'x':
            *basep = 16;
            break;
        case 't': case 'n':
            *basep = 10;
            break;
        case 'o':
            *basep = 8;
            break;
        default:
            *basep = 10;
            return (p);
        }
        return (p + 2);
    }
    *basep = 10;
    return (p);
}


/*
 *  _atob(vp,p,base)
 */
static int
#ifdef HAVE_QUAD
_atob (u_quad_t *vp, char *p, int base)
{
    u_quad_t value, v1, v2;
#else
_atob (unsigned long *vp, char *p, int base)
{
    unsigned long value, v1, v2;
#endif
    char *q, tmp[20];
    int digit;

    if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
        base = 16;
        p += 2;
    }

    if (base == 16 && (q = s_strchr(p, '.')) != 0) {
        if (q - p > sizeof(tmp) - 1)
            return (0);

        s_strncpy(tmp, p, q - p);
        tmp[q - p] = '\0';
        if (!_atob (&v1, tmp, 16))
            return (0);

        q++;
        if (s_strchr(q, '.'))
            return (0);

        if (!_atob (&v2, q, 16))
            return (0);
        *vp = (v1 << 16) + v2;
        return (1);
    }

    value = *vp = 0;
    for (; *p; p++) {
        if (*p >= '0' && *p <= '9')
            digit = *p - '0';
        else if (*p >= 'a' && *p <= 'f')
            digit = *p - 'a' + 10;
        else if (*p >= 'A' && *p <= 'F')
            digit = *p - 'A' + 10;
        else
            return (0);

        if (digit >= base)
            return (0);
        value *= base;
        value += digit;
    }
    *vp = value;
    return (1);
}

int atob(int *vp, char *p, int base)
{
#ifdef HAVE_QUAD
    u_quad_t v;
#else
    unsigned long  v;
#endif

    if (base == 0)
        p = _getbase (p, &base);
    if (_atob (&v, p, base)) {
        *vp = v;
        return (1);
    }
    return (0);
}

int __vsscanf (const char *buf, const char *s, va_list ap)
{
    int             count, noassign, width, base, lflag;
    const char     *tc;
    char           *t, tmp[512];

    count = noassign = width = lflag = 0;
    while (*s && *buf) {
    while (_isspace (*s))
        s++;
    if (*s == '%') {
        s++;
        for (; *s; s++) {
        if (s_strchr ("dibouxcsefg%", *s))
            break;
        if (*s == '*')
            noassign = 1;
        else if (*s == 'l' || *s == 'L')
            lflag = 1;
        else if (*s >= '1' && *s <= '9') {
            for (tc = s; _isdigit (*s); s++);
            s_strncpy (tmp, tc, s - tc);
            tmp[s - tc] = '\0';
            atob (&width, tmp, 10);
            s--;
        }
        }
        if (*s == 's') {
        while (_isspace (*buf))
            buf++;
        if (!width)
            width = s_strcspn (buf, ISSPACE);
        if (!noassign) {
            s_strncpy (t = va_arg (ap, char *), buf, width);
            t[width] = '\0';
        }
        buf += width;
        } else if (*s == 'c') {
        if (!width)
            width = 1;
        if (!noassign) {
            s_strncpy (t = va_arg (ap, char *), buf, width);
            t[width] = '\0';
        }
        buf += width;
        } else if (s_strchr ("dobxu", *s)) {
        while (_isspace (*buf))
            buf++;
        if (*s == 'd' || *s == 'u')
            base = 10;
        else if (*s == 'x')
            base = 16;
        else if (*s == 'o')
            base = 8;
        else if (*s == 'b')
            base = 2;
        if (!width) {
            if (_isspace (*(s + 1)) || *(s + 1) == 0)
            width = s_strcspn (buf, ISSPACE);
            else
            width = s_strchr (buf, *(s + 1)) - buf;
        }
        s_strncpy (tmp, buf, width);
        tmp[width] = '\0';
        buf += width;
        if (!noassign)
            atob((int*)va_arg (ap, unsigned int *), tmp, base);
        }
        if (!noassign)
        count++;
        width = noassign = lflag = 0;
        s++;
    } else {
        while (_isspace (*buf))
        buf++;
        if (*s != *buf)
        break;
        else
        s++, buf++;
    }
    }
    return (count);
}

extern "C" int sscanf(const char *str, const char *format, ...)
{
    va_list ap;
    int ret;

    va_start(ap, format);
    ret = __vsscanf(str, format, ap);
    va_end(ap);
    return(ret);
}