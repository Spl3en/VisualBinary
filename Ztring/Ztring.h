/**
 *  @author	 :   Spl3en
 *  @file	   :   Ztring.h
 *  @version	:   1.6
 *
 *  Ztring offre un ensemble de fonctionnalitées de manipulation de chaînes de caractères et de fichiers.
 *
 *  Changelog :
 *
 *		[+] 1.1
 *
 *		  [ADD]	   str_dup_printf
 *		  [ADD]	   str_substring
 *		  [ADD]	   str_debug
 *		  [ADD]	   file_isset
 *		  [ADD]	   file_save_binary
 *		  [ADD]	   str_sort_by_alpha
 *		  [ADD]	   str_b64_encode
 *		  [ADD]	   str_b64_decode
 *
 *	  [+] 1.2
 *		  [ADD]	   file_is_word_exists (DEPRECATED en 1.5)
 *
 *	  [+] 1.3
 *		  [ADD]	   ztring_new, ztring_new_with_text
 *		  [ADD]	   ztring functions
 *
 *	  [+] 1.4
 *		  [FIX]	   Optimisation de str_replace et autres algo de recherches
 *
 *	  [+] 1.5
 *		  [RMV]	   file_is_word_exists retirée (buggée)
 *		  [FIX]	   str_pos fonctionne désormais commme la fonction php et non plus comme strstr du C
 *		  [ADD]	   str_explode
 *		  [ADD]	   file_get_size
 *
 *	  [+] 1.6			(10 / 01 / 2012)
 *		  [ADD]	   ztr_concat_nth / ztr_concat_ztring_nth
*/

#ifndef ztring_H_INCLUDED
#define ztring_H_INCLUDED

/* Librairies */
#include <stdarg.h>
#include <string.h>
#include "../BbQueue/BbQueue.h"

/* Macros */
#define str_make_macro(s) #s
#define TOSTRING(x) str_make_macro(x)
#define str_equals(str1, str2) (strcmp((str1), (str2)) == 0)

/* Defines */
#define FILE_APPEND ((void*)1)
#define EOS		 (-1)

typedef
struct _Ztring
{
	BbQueue *_text;

}	Ztring;

typedef
struct _Buffer
{
	int size;
	unsigned char *data;
}	Buffer;

typedef
struct _File
{
	char *data;
	int size;

	char *filename;

}	File;

	/** * * * * * * * *
	*  @Constructors  *
	* * * * * * * * * */

Ztring *
ztring_new			  	(void);

Ztring *
ztring_new_with_text	(char *text);

Buffer *
buffer_new (int size);

Buffer *
buffer_dup (Buffer *buf);

Buffer *
buffer_new_ptr (unsigned char *ptr, int size);

Buffer *
buffer_new_ptr_noalloc (unsigned char *ptr, int size);


	/** * * * * * * * *
	*	 @Methods	*
	* * * * * * * * * */
/* * Buffer * */


/* * Ztring * */
void
ztring_concat_letter	(Ztring *z, unsigned char c);

char *
ztring_get_text		 	(Ztring *z);

char *
ztring_get_text_reversed (Ztring *z);

void
ztring_get_text_buffer 	(Ztring *z, char *buffer, int maxsize);

void
ztring_concat		   	(Ztring *z, char *text);

void
ztring_concat_letter_nth (Ztring *z, char c, int nth);

void
ztring_concat_nth	   	(Ztring *z, char *text, int nth);

void
ztring_concat_ztring	(Ztring *ztr1, Ztring *ztr2);

void
ztring_concat_ztring_nth (Ztring *ztr1, Ztring *ztr2, int nth);

void
ztring_print_text	   	(Ztring *z);

int
ztring_get_len		  	(Ztring *z);

void
ztring_debug			(Ztring *z);
/* * String * */
char *
str_repeat (char *repeat_pattern, int n);

int
str_hex (char *str);

char *
str_trim				(char *str);

char *
str_malloc_clear		(int size);

char *
str_replace			 (const char *search, const char *replace, char *string);

char *
str_dup_printf		  (const char *format, ...);

int
str_substring		   (const char *str, int start, int end, char *res);

int
str_getline			 (char *str, char *dest, int maxlen, int pos);

void
str_cpy				 (char **dest, const char *str);

void
strn_cpy				(char *dest, const char *str, int size);

void
str_gets				(char *buffer, int len);

int
str_pos				 (const char *str, const char *search);

int
str_pos_reverse 		(const char *str, const char *search);

void
str_debug			   (const char *str);

void
str_debug_len		   (const unsigned char *str, int len);

char *
str_bet				 (const char *str, const char *start, const char *end);

void
str_bet_buffer 			(const char *str, const char *start, const char *end, char *buffer);

int
str_sort_by_alpha	   (char *str1, char *str2);

char *
str_dup_to_lower		(char *str);

void
str_to_lower			(char *str);

int
str_get_word			(char *str, char *dest, int maxlen);

int
str_is_empty			(char *str);

BbQueue *
str_explode			 (char *str, const char *delimiter);

int
is_letter (char c);

/* * File * */
char *
file_get_extension	  (const char *filename);

FILE *
file_open			   (const char *filename, const char *open_mode);

void
file_clear			  (const char *filename);

int
file_save_binary		(const char *filename, const char *data, int size);

void
file_put_contents	   (const char *filename, const char *text, void *type);

char *
file_get_contents	   (const char *filename);

unsigned int
file_get_size		   (const char *filename);

unsigned int
file_get_size_handler 	(FILE *handler);

char *
file_getline 			(FILE *file);

int
file_get_lines_count 	(char *filename);

char *
file_get_contents_line  (const char *filename, int *line);

	/** * * * * * * * *
	*   @Destructors  *
	* * * * * * * * * */

void
ztring_free	 (Ztring *z);

void
ztring_clear	(Ztring *z);

char *
ztring_release (Ztring *z);

void
buffer_free (Buffer *b);


#endif
