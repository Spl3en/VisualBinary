/**
*	@author 	:	Spl3en
*	@file 		:	Ztring.c
*
*	Furthermore informations in Ztring.h
*/

#include "Ztring.h"

/**
*	Private Functions
*/
static int
str_sort_by_alpha_ex (char *str1, char *str2, int n)
{
	// Return 1 for switch, else 0
	int len1 = strlen(str1);
	int len2 = strlen(str2);

	if (n > len1)
		return 0;

	if (n > len2)
		return 1;

	if (str1[n] < str2[n])
		return 0;

	else if (str1[n] > str2[n])
		return 1;

	else
		return str_sort_by_alpha_ex(str1, str2, ++n);
}

static char
b64_encode (unsigned char u)
{
	if (u < 26)  return 'A' + u;
	if (u < 52)  return 'a' + (u - 26);
	if (u < 62)  return '0' + (u - 52);
	if (u == 62) return '+';

	return '/';
}

static unsigned char
b64_decode (char c)
{
	if (c >= 'A' && c <= 'Z') return (c - 'A');
	if (c >= 'a' && c <= 'z') return (c - 'a' + 26);
	if (c >= '0' && c <= '9') return (c - '0' + 52);
	if (c == '+')			 return 62;

	return 63;
}

static int
is_base64 (char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
		(c >= '0' && c <= '9') || (c == '+')			 ||
		(c == '/')			 || (c == '='))
	{
		return 1;
	}

	return 0;
}

/**
*	Public Functions
*/
Ztring *
ztring_new (void)
{
	Ztring *z = malloc(sizeof(Ztring));
	z->_text = bb_queue_new();

	return z;
}

Buffer *
buffer_new (int size)
{
	Buffer *b;

	if ((b = malloc(sizeof(Buffer))) == NULL)
		return NULL;

	b->size = size;
	b->data = malloc(size);

	return b;
}

Buffer *
buffer_dup (Buffer *buf)
{
	return buffer_new_from_ptr(buf->data, buf->size);
}

Buffer *
buffer_new_from_ptr (unsigned char *ptr, int size)
{
	Buffer *b = buffer_new(size);
	memcpy(b->data, ptr, size);

	return b;
}

Buffer *
buffer_new_from_ptr_noalloc (unsigned char *ptr, int size)
{
	Buffer *b = NULL;

	if ((b = malloc(sizeof(Buffer))) == NULL)
		return NULL;

	b->size = size;
	b->data = ptr;

	return b;
}

void
buffer_free (Buffer *b)
{
	if (b)
	{
		free(b->data);
		free(b);
	}
}


Ztring *
ztring_new_with_text (char *text)
{
	Ztring *z = ztring_new();

	ztring_concat(z, text);

	return z;
}

int
ztring_get_len(Ztring *z)
{
	return bb_queue_get_length(z->_text);
}

void
ztring_print_text (Ztring *z)
{
	printf("%s", ztring_get_text(z));
}

void
ztring_concat_letter (Ztring *z, unsigned char c)
{
	bb_queue_add_raw (z->_text, (void *) (int) c);
}

void
ztring_concat_letter_nth (Ztring *z, char c, int nth)
{
	bb_queue_add_nth(z->_text, (void *) (int) c, nth);
}

void
ztring_concat_ztring_nth (Ztring *ztr1, Ztring *ztr2, int nth)
{
	bb_queue_concat_nth (ztr1->_text, ztr2->_text, nth);
}

void
ztring_concat_ztring(Ztring *ztr1, Ztring *ztr2)
{
	bb_queue_concat(ztr1->_text, ztr2->_text);
}

void
ztring_concat_nth (Ztring *z, char *text, int nth)
{
	int len = strlen(text);
	int i;

	for (i = 0; i < len; i++)
	{
		ztring_concat_letter_nth(z, (char)(int)text[i], nth);
	}
}

void
ztring_concat (Ztring *z, char *text)
{
	int len = strlen(text);
	int i;

	for (i = 0; i < len; i++)
	{
		ztring_concat_letter(z, (char)(int)text[i]);
	}
}

char *
ztring_get_text(Ztring *z)
{
	char *text = malloc(bb_queue_get_length(z->_text) + 1);
	int index = 0;

	foreach_bbqueue_item_raw (z->_text, char c)
	{
		text[index++] = c;
	}

	text[index] = '\0';

	return text;
}

char *
ztring_get_text_reversed (Ztring *z)
{
	char *text = malloc(bb_queue_get_length(z->_text) + 1);
	int index = 0;

	foreach_bbqueue_item_reversed_raw (z->_text, char c)
	{
		text[index++] = c;
	}

	text[index] = '\0';

	return text;
}

void
ztring_get_text_buffer (Ztring *z, char *buffer, int maxsize)
{
	int index = 0;

	foreach_bbqueue_item (z->_text, void *c)
	{
		if (index < maxsize)
			buffer[index++] = (char)(int) c;
		else
			break_foreach;
	}

	buffer[index] = '\0';
}

char *
str_dup_printf (const char *format, ...)
{
	char *str = NULL;

	if (format)
	{
		char t[1];

		va_list pa;
		size_t size = 0;

		va_start (pa, format);
		size = vsnprintf (t, 1, format, pa);
		size++;
		str = malloc (size + 1);
		memset(str, '\0', size + 1);

		if (str)
			vsnprintf (str, size, format, pa);
	}

	return str;
}

int
str_get_word(char *str, char *dest, int maxlen)
{
	int len = strlen(str);
	int i;
	int bufpos = 0;

	if (maxlen == -1)
		maxlen = len;

	for (i = 0; i < len; i++)
	{
		if (str[i] != ' ')
			dest[bufpos++] = str[i];

		else
			break;
	}

	dest[bufpos] = '\0';
	return i;
}


int
str_getline (char *str, char *dest, int maxlen, int pos)
{
	int len = strlen(str);
	int i;
	int bufpos = 0;
	char c;

	for (i = pos ; i < len; i++)
	{
		if (bufpos >= maxlen)
			return i;

		c = str[i];
		dest[bufpos++] = str[i];

		if (c == '\n')
		{
			dest[--bufpos] = '\0';
			return i + 1;
		}
	}

	dest[bufpos] = '\0';
	return EOS;
}

char *
file_getline (FILE *file)
{
	int bufpos = 0;
	char c;

	long int cur = ftell(file);

	for (c = fgetc(file); c != '\n' && c != EOF; c = fgetc(file))
		bufpos++;

	char *dest = malloc(bufpos + 1);
	dest[bufpos] = '\0';

	fseek(file, cur, SEEK_SET);

	bufpos = 0;

	for (c = fgetc(file); c != '\n' && c != EOF; c = fgetc(file))
		dest[bufpos++] = c;

	return dest;
}

int
str_is_empty(char *msg)
{
	return (msg[0] == '\0');
}

void
str_gets(char *buffer, int len)
{
	fgets(buffer, len, stdin);
	buffer[strlen(buffer) - 1] = '\0';
}

void
str_cpy (char **a, const char *b)
{
	*a = malloc(strlen(b) + 1);
	strcpy(*a, b);
}

void
strn_cpy (char *dest, const char *str, int size)
{
	strncpy(dest, str, size);
	dest[size-1] = '\0';
}

int
str_substring (const char *str, int start, int end, char *res)
{
	if (start < end)
	{
		int len = end - start;
		res[len] = '\0';
		memcpy(res, str + start, len);

		return (len);
	}

	else
	{
		printf("Error in substring :\n"
			   "start = %d - end = %d\n"
			   "String = %s\n", start, end, str);

		return -1;
	}
}

int
str_is_space(char s)
{
	return (s == ' ');
}

char *
str_trim (char *str)
{
	char *ibuf = str, *obuf = str;
	int i = 0, cnt = 0;

	if (!str)
		return NULL;

	for
	(
		 ibuf = str;
		 *ibuf && str_is_space(*ibuf);
		 ++ibuf
	)
		;

	if (str != ibuf)
		memmove(str, ibuf, ibuf - str);

	while (*ibuf)
	{
		if (str_is_space(*ibuf) && cnt)
			ibuf++;

		else
		{
			if (!str_is_space(*ibuf))
				cnt = 0;

			else
			{
				*ibuf = ' ';
				cnt = 1;
			}

			obuf[i++] = *ibuf++;
		}
	}

	obuf[i] = '\0';

	while (--i >= 0)
	{
		if (!str_is_space(obuf[i]))
			break;
	}

	obuf[++i] = '\0';

	return str;
}

char *
str_replace (const char *search, const char *replace, char *string)
{
	int len_string  = strlen(string);
	int len_search  = strlen(search);
	int len_replace = strlen(replace);
	int len_offset  = (len_search - len_replace);
	int size_offset = (len_replace - len_search);
	int len_dest = -1;
	int start = 0;
	int count = 0;
	int r = 0, i;

	char *dest = NULL;

	BbQueue *pos_list = bb_queue_new();

	/* Parsing occurences */
	for (i = 0; i < len_string; i++)
	{
		if (string[i] == search[count])
			count++;

		else
			count = 0;

		if (count == len_search)
		{
			bb_queue_add(pos_list, (void*)((int)(i + 1) - len_search));
			count = 0;
		}
	}

	/* Compute string destination size */
	len_dest = len_string + (bb_queue_get_length(pos_list) * size_offset);
	dest = malloc(len_dest + 1);

	if (bb_queue_get_length(pos_list) == 0)
	{
		strncpy(dest, string, len_dest + 1);
		return dest;
	}

	/* Fill the destination string and replace the occurences */
	count = 0;
	while (bb_queue_get_length(pos_list) > 0)
	{
		r = (int) bb_queue_get_first(pos_list);
		size_offset = len_offset * count;

		for (
			 i = start;
			 i < r - size_offset;
			 i++
		)
		{
			dest[i] = string[i + size_offset];
		}
		dest[i] = '\0';

		strcat(dest, replace);

		start = i + len_replace;
		count++;
	}

	/* Fill the end of the string */
	for (
		i = len_string - 1, count = len_dest - 1;
		i > r + len_search - 1;
		i--, count--
	)
		dest[count] = string[i];

	dest[len_dest] = '\0';

	return dest;
}

char *
str_malloc_clear (int size)
{
	char *str = malloc(size);
	str[0] = '\0';

	return str;
}

void
str_cat (char **dest, const char *cat)
{
	char *res = malloc(strlen(*dest) + strlen(cat) + 1);
	strcat(res, cat);

	free(*dest);
	*dest = res;
}

BbQueue *
str_explode (char *str, const char *delimiter)
{
	BbQueue *q = NULL;
	int pos = -1;
	int len_delimiter = strlen(delimiter);
	int len = strlen(str);

	char *tmp = NULL;

	while (
        ((pos = str_pos(str, delimiter)) != -1)
    &&  (pos != 0)
    &&  (pos <= len)
    )
	{
        if (q == NULL)
            q = bb_queue_new();

        tmp = malloc(pos);
        strncpy(tmp, str, pos);
        tmp[pos-1] = '\0';
        bb_queue_add(q, tmp);

        str = str + pos + len_delimiter;

	}

    if (tmp == NULL)
        return NULL;

	str_cpy(&tmp, str);
	bb_queue_add(q, tmp);

	return q;
}

char *
file_get_contents (const char *filename)
{
	FILE *f = NULL;
	char *ret = NULL;
	char c;
	int size = 0;

	f = fopen(filename, "r");

	if (!f)
	{
		printf("%s cannot be opened\n", filename);
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	size = ftell(f);

	rewind(f);

	ret = malloc(size + 1);
	size = 0;

	while ((c = fgetc(f)) != EOF)
		ret[size++] = c;

	ret[size] = '\0';

	fclose(f);

	return ret;
}
char *
file_get_contents_line (const char *filename, int *line)
{
	FILE *f = NULL;
	char *ret = NULL;
	char c;
	int size = 0;
	*line = 1;

	f = fopen(filename, "r");

	if (!f)
	{
		printf("%s cannot be opened\n", filename);
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	size = ftell(f);

	rewind(f);

	ret = malloc(size + 1);
	size = 0;

	while ((c = fgetc(f)) != EOF)
	{
		ret[size++] = c;

		if (c == '\n')
			*line = *line + 1;
	}

	ret[size] = '\0';

	fclose(f);

	return ret;
}

char *
file_get_contents_handle (FILE *f)
{
	char *ret = NULL;
	char c;
	int size = 0;
	fpos_t pos;

	fgetpos(f, &pos);
	fseek(f, 0, SEEK_END);
	size = ftell(f);

	fsetpos(f, &pos);

	ret = malloc(size + 1);
	size = 0;

	while ((c = fgetc(f)) != EOF)
		ret[size++] = c;

	ret[size] = '\0';
	fclose(f);

	return ret;
}

unsigned int
file_get_size(const char *filename)
{
	FILE *f = NULL;
	unsigned int size = 0;

	f = fopen(filename, "r");

	if (!f)
	{
		printf("%s cannot be opened\n", filename);
		return -1;
	}

	fseek(f, 0, SEEK_END);
	size = ftell(f);

	fclose(f);

	return size;
}

unsigned int
file_get_size_handler (FILE *handler)
{
	unsigned int size = 0;

	fseek(handler, 0, SEEK_END);
	size = ftell(handler);
	rewind(handler);

	return size;
}

int
file_get_lines_count (char *filename)
{
	FILE* f = fopen(filename, "r");
	int ch, lines = 0;

	if (!f)
	{
		printf("%s cannot be opened\n", filename);
		return -1;
	}

	do
	{
		ch = fgetc(f);
		if(ch == '\n')
			lines++;
	} while (ch != EOF);

	if (ch != '\n' && lines != 0)
		lines++;

	fclose(f);

	return lines;
}

char *
file_get_extension (const char *filename)
{
	return strchr(filename, '.');
}

void
file_clear(const char *filename)
{
	FILE *fp = file_open(filename, "w+");
	fclose(fp);
}

FILE *
file_open (const char *filename, const char *open_mode)
{
	FILE *fp = fopen(filename, open_mode);

	if (!fp)
	{
		printf("%s : cannot be opened\n", filename);
		return NULL;
	}

	return fp;
}

void
file_put_contents (const char *filename, const char *text, void *type)
{
	char *open_type = NULL;
	FILE *f = NULL;

	open_type = (type == FILE_APPEND) ?
		"a+" : "w+";

	f = file_open(filename, open_type);

	fprintf(f, "%s", text);
	fflush(f);

	fclose(f);
}

void
str_to_lower(char *str)
{
	int len = strlen(str);
	int i;

	for (i = 0; i < len; i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] = str[i] + 32;
		}
	}
}

char *
str_dup_to_lower(char *str)
{
	int len = strlen(str);
	int i;
	char *ret = malloc(len + 1);
	ret[len] = '\0';

	for (i = 0; i < len; i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			ret[i] = str[i] + 32;
		}
		else
			ret[i] = str[i];
	}

	return ret;
}

int
str_hex (char *str)
{
	if (str == NULL)
		return 0;

	return (strtol(str, NULL, 16));
}

int
str_sort_by_alpha (char *str1, char *str2)
{
	return str_sort_by_alpha_ex(str1, str2, 0);
}

int
file_save_binary (const char *filename, const char *data, int size)
{
	FILE *fp = fopen(filename, "wb");

	if (fp == NULL)
		return 0;

	fwrite(data, sizeof(*data), size, fp);
	fclose(fp);

	return 1;
}

char *
str_pos_ptr (char *str, const char *search)
{
    int pos = str_pos(str, search);

    if (pos >= 0)
        return &str[pos];

    return NULL;
}

char *
str_pos_after_ptr (char *str, const char *search)
{
    int pos = str_pos_after(str, search);

    if (pos >= 0)
        return &str[pos];

    return NULL;
}

int
str_pos (const char *str, const char *search)
{
	int i,  len_string = strlen(str),
            len_search = strlen(search);
	int count = 0;


	for (i = 0; i < len_string; i++)
	{
		if (str[i] == search[count])
			count++;

		else
			count = 0;

		if (count == len_search)
		{
			return i - len_search + 1;
		}
	}

	return -1;
}

int
str_pos_after (const char *str, const char *search)
{
    int res = str_pos(str, search);
    if (res != -1)
    {
        res += strlen(search);
    }

    return res;
}

int
str_pos_reverse (const char *str, const char *search)
{
	int i, len_string = strlen(str), len_search = strlen(search);
	int count = len_search - 1;

	for (i = len_string - 1; i >= 0; i--)
	{
		if (str[i] == search[count])
			count--;
		else
			count = len_search - 1;

		if (count == -1)
			return i;
	}

	return -1;
}


char *
str_bet (const char *str, const char *start, const char *end)
{
	int len;
	char *ret = NULL;
	int start_len = strlen(start);

	int pos_end;
	int pos_start = str_pos(str, start);

	if (pos_start == -1)
		return NULL;

	if (end != (void*)-1)
		pos_end = str_pos(&str[pos_start + start_len], end) + pos_start + start_len;
	else
		pos_end = strlen(str);

	if (pos_end == -1)
		return NULL;

	len = (pos_end) - (pos_start + start_len);
	ret = malloc(len + 1);

	str_substring
	(
		str,
		pos_start + start_len,
		pos_end,
		ret
	);

	return ret;
}

void
str_bet_buffer (const char *str, const char *start, const char *end, char *buffer)
{
	int start_len = strlen(start);

	int pos_end;
	int pos_start = str_pos(str, start);


	if (pos_start == -1)
		return;

	pos_end = str_pos(&str[pos_start + start_len], end) + pos_start + start_len;

	if (pos_end == -1)
		return;

	str_substring
	(
		str,
		pos_start + start_len,
		pos_end,
		buffer
	);
}

char *
str_b64_encode (int size, unsigned char *src)
{
	int i;
	char *out, *p;
	unsigned char b1, b2, b3, b4, b5, b6, b7;

	if (!src)
		return NULL;

	if (!size)
		size = strlen ((char *)src);

	out = malloc(size * 4/3 + 4);

	p = out;

	for (i = 0; i < size; i += 3)
	{
		b1 = 0; b2 = 0; b3 = 0; b4 = 0; b5 = 0; b6 = 0; b7 = 0;
		b1 = src[i];

		if (i + 1 < size)
			b2 = src[i + 1];

		if (i + 2 < size)
			b3 = src[i + 2];

		b4 = b1 >> 2;
		b5 = ((b1 & 0x3) << 4) | (b2 >> 4);
		b6 = ((b2 & 0xf) << 2) | (b3 >> 6);
		b7 = b3 & 0x3f;

		*p++ = b64_encode(b4);
		*p++ = b64_encode(b5);

		if (i + 1 < size)
			*p++ = b64_encode(b6);
		else
			*p++ = '=';

		if (i + 2 < size)
			*p++ = b64_encode(b7);
		else
			*p++ = '=';
	}

	return out;

}

int
str_b64_decode(unsigned char *dest, const char *src)
{
	char c1, c2, c3, c4;
	unsigned char *p = dest;
	unsigned char *buf= NULL;
	unsigned char b1, b2, b3, b4;
	int k, l;

	if (src && *src)
	{
		l = strlen(src) + 1;
		buf = malloc(l);

		for (k = 0, l = 0; src[k]; k++)
		{
			if(is_base64(src[k]))
				buf[l++]= src[k];
		}

		for(k=0; k<l; k+=4)
		{

			c1 = 'A';
			c2 = 'A';
			c3 = 'A';
			c4 = 'A';

			b1 = 0;
			b2 = 0;
			b3 = 0;
			b4 = 0;

			c1= buf[k];

			if (k + 1 < l)
				c2 = buf[k+1];

			if (k + 2 < l)
				c3 = buf[k+2];

			if (k + 3 < l)
				c4 = buf[k+3];

			b1 = b64_decode(c1);
			b2 = b64_decode(c2);
			b3 = b64_decode(c3);
			b4 = b64_decode(c4);

			*p++ = (b1 << 2) | (b2 >> 4);

			if(c3 != '=')
				*p++ = ((b2 & 0xf) << 4) | (b3 >> 2);

			if(c4 != '=') {
				*p++ = ((b3 & 0x3) << 6) | b4;
			}
		}

		free(buf);

		return (p - dest);
	}

	return 0;

}

void
str_debug (const char *str)
{
	int i, len = strlen(str);
	for (i = 0; i < len; i++)
	{
		printf("(%d)[%x] = [%c]\n", i, str[i], str[i]);
	}
}

char *
str_repeat (char *repeat_pattern, int n)
{
	int pattern_len = strlen(repeat_pattern);

	char *res = malloc(pattern_len * n + 1);

	for (int i = 0; i < n; i++)
	{
		strcpy(&res[i*pattern_len], repeat_pattern);
	}

	return res;
}

void
str_debug_len (const unsigned char *str, int len)
{
	printf("String debug Data :\n");

	for (int i = 0; i < len; i++)
	{
		printf("%c", str[i]);
	}

	printf("\n");

	for (int i = 0; i < len; i++)
	{
		printf("%3c ", str[i]);
	}

	printf("\n");

	for (int i = 0; i < len; i++)
	{
		printf("  %.2x", str[i]);
	}

	printf("\n");

	for (int i = 0; i < len; i++)
	{
		printf("%3d ", i);
	}

	printf("\n");
}

int
is_letter (char c)
{
	return (
		(c >= 'A' && c <= 'Z')
	||  (c >= 'a' && c <= 'z')
	);
}

void
ztring_debug (Ztring *z)
{
	char *buffer = ztring_get_text(z);
	str_debug_len(buffer, ztring_get_len(z));
	free(buffer);
}

void
ztring_clear(Ztring *z)
{
	if (z != NULL)
	{
		bb_queue_clear(z->_text);
	}
}

char *
ztring_release (Ztring *z)
{
	char *str = ztring_get_text(z);
	ztring_free(z);
	return str;
}


void
ztring_free (Ztring *z)
{
	if (z != NULL)
	{
		bb_queue_clear(z->_text);
		free(z);
	}
}
