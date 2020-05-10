/*
 * jit-varint.c - Variable length integer encoding.
 *
 * Copyright (C) 2011  Aleksey Demakov
 *
 * The libjit library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * The libjit library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the libjit library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <jit/jit.h>
#include "jit-varint.h"

/*
 * Flush the current encode buffer.
 */
static int
flush_encoder(jit_varint_encoder_t *encoder)
{
	jit_varint_data_t data;

	/* Allocate a new jit_varint_data structure to hold the data */
	data = jit_malloc(sizeof(struct jit_varint_data) + encoder->len);
	if(!data)
	{
		return 0;
	}

	/* Copy the temporary debug data into the new structure */
	jit_memcpy(data->data, encoder->buf, encoder->len);

	/* Link the structure into the debug list */
	data->next = 0;
	if(encoder->last)
	{
		encoder->last->next = data;
	}
	else
	{
		encoder->data = data;
	}
	encoder->last = data;

	/* Reset the temporary debug buffer */
	encoder->len = 0;
	return 1;
}

void
_jit_varint_init_encoder(jit_varint_encoder_t *encoder)
{
	encoder->len = 0;
	encoder->data = 0;
	encoder->last = 0;
}

void
_jit_varint_init_decoder(jit_varint_decoder_t *decoder, jit_varint_data_t data)
{
	decoder->len = 0;
	decoder->data = data;
	decoder->end = (data == 0);
}

int
_jit_varint_encode_end(jit_varint_encoder_t *encoder)
{
	if(!encoder->len)
	{
		return 1;
	}

	/* Mark the end of the data */
	encoder->buf[encoder->len++] = 0xFF;

	/* Flush the data that we have collected so far */
	return flush_encoder(encoder);
}

/*
 * Compress a "long" value so that it takes up less bytes.
 * This is used to store offsets within functions and
 * debug line numbers, which are usually small integers.
 */
int
_jit_varint_encode_uint(jit_varint_encoder_t *encoder, jit_uint value)
{
	if(encoder->len + 6 > sizeof(encoder->buf))
	{
		/* Overflow occurred: end current buffer */
		if(!_jit_varint_encode_end(encoder))
		{
			return 0;
		}
	}

	/* Write the value to the temporary buffer */
	if(value < 0x80)
	{
		/* 0xxx xxxx */
		encoder->buf[encoder->len++] = (unsigned char) (value);
	}
	else if(value < 0x4000)
	{
		/* 10xx xxxx | xxxx xxxx */
		encoder->buf[encoder->len++] = (unsigned char) (value >> 8) | 0x80;
		encoder->buf[encoder->len++] = (unsigned char) (value);
	}
	else if(value < 0x200000)
	{
		/* 110x xxxx | xxxx xxxx | xxxx xxxx */
		encoder->buf[encoder->len++] = (unsigned char) (value >> 16) | 0xC0;
		encoder->buf[encoder->len++] = (unsigned char) (value >> 8);
		encoder->buf[encoder->len++] = (unsigned char) (value);
	}
	else if(value < 0x10000000)
	{
		/* 1110 xxxx | xxxx xxxx | xxxx xxxx | xxxx xxxx */
		encoder->buf[encoder->len++] = (unsigned char) (value >> 24) | 0xE0;
		encoder->buf[encoder->len++] = (unsigned char) (value >> 16);
		encoder->buf[encoder->len++] = (unsigned char) (value >> 8);
		encoder->buf[encoder->len++] = (unsigned char) (value);
	}
	else
	{
		/* 1111 0000 | xxxx xxxx | xxxx xxxx | xxxx xxxx | xxxx xxxx */
		encoder->buf[encoder->len++] = 0xF0;
		encoder->buf[encoder->len++] = (unsigned char) (value >> 24);
		encoder->buf[encoder->len++] = (unsigned char) (value >> 16);
		encoder->buf[encoder->len++] = (unsigned char) (value >> 8);
		encoder->buf[encoder->len++] = (unsigned char) (value);
	}

	return 1;
}

jit_varint_data_t
_jit_varint_get_data(jit_varint_encoder_t *encoder)
{
	return encoder->data;
}

void
_jit_varint_free_data(jit_varint_data_t data)
{
	while(data)
	{
		jit_varint_data_t next = data->next;
		jit_free(data);
		data = next;
	}
}

int
_jit_varint_decode_end(jit_varint_decoder_t *decoder)
{
	return decoder->end;
}

jit_uint
_jit_varint_decode_uint(jit_varint_decoder_t *decoder)
{
	jit_uint value;
	unsigned char c;

	value = (jit_uint) (-1);
	if(decoder->end)
	{
		return value;
	}
	if(decoder->len >= JIT_VARINT_BUFFER_SIZE)
	{
		/* Sanity check failed */
		decoder->end = 2;
		return value;
	}

again:
	c = decoder->data->data[decoder->len++];
	if(!(c & 0x80))
	{
		/* 0xxx xxxx */
		value = ((jit_uint) c);
	}
	else if(!(c & 0x40))
	{
		/* 10xx xxxx | xxxx xxxx */
		value = ((jit_uint) c & 0x3F) << 8;
		value |= ((jit_uint) decoder->data->data[decoder->len++]);
	}
	else if(!(c & 0x20))
	{
		/* 110x xxxx | xxxx xxxx | xxxx xxxx */
		value = ((jit_uint) c & 0x1F) << 16;
		value |= ((jit_uint) decoder->data->data[decoder->len++]) << 8;
		value |= ((jit_uint) decoder->data->data[decoder->len++]);
	}
	else if(!(c & 0x10))
	{
		/* 1110 xxxx | xxxx xxxx | xxxx xxxx | xxxx xxxx */
		value = ((jit_uint) c & 0x0F) << 24;
		value |= ((jit_uint) decoder->data->data[decoder->len++]) << 16;
		value |= ((jit_uint) decoder->data->data[decoder->len++]) << 8;
		value |= ((jit_uint) decoder->data->data[decoder->len++]);
	}
	else if(!(c & 0x0f))
	{
		/* 1111 0000 | xxxx xxxx | xxxx xxxx | xxxx xxxx | xxxx xxxx */
		value |= ((jit_uint) decoder->data->data[decoder->len++]) << 24;
		value |= ((jit_uint) decoder->data->data[decoder->len++]) << 16;
		value |= ((jit_uint) decoder->data->data[decoder->len++]) << 8;
		value |= ((jit_uint) decoder->data->data[decoder->len++]);
	}
	else
	{
		/* Met data end mark */
		if(decoder->data->next)
		{
			/* There is another data block */
			decoder->data = decoder->data->next;
			decoder->len = 0;
			goto again;
		}

		/* This was the last data block */
		decoder->end = 1;
	}

	return value;
}

#if 0

void
_jit_varint_encode_int(jit_varint_encoder_t *encoder, jit_int value)
{
	value = (value << 1) ^ (value >> (8 * sizeof(jit_uint) - 1));;
	_jit_varint_encode_uint(encoder, (jit_uint) value);
}

jit_int
_jit_varint_decode_int(jit_varint_decoder_t *decoder)
{
	jit_uint value = _jit_varint_decode_uint(decoder);
	return (jit_int) ((value >> 1) ^ -(value & 1));
}

#endif

#if defined(VARINT_TEST)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *
jit_malloc(unsigned int size)
{
	return malloc(size);
}

void
jit_free(void *ptr)
{
	if(ptr)
	{
		free(ptr);
	}
}

void *
jit_memcpy(void *dest, const void *src, unsigned int len)
{
	return memcpy(dest, src, len);
}

int
main()
{
	jit_uint i, j, n, v;

	for(n = 1; n != 0; n >>= 1)
	{
		jit_varint_encoder_t encoder;
		_jit_varint_init_encoder(&encoder);
		_jit_varint_encode_uint(&encoder, n);
		_jit_varint_encode_uint(&encoder, n - 1);
		_jit_varint_encode_uint(&encoder, n + 1);
		_jit_varint_encode_end(&encoder);

		jit_varint_data_t data = _jit_varint_get_data(&encoder);

		jit_varint_decoder_t decoder;
		_jit_varint_init_decoder(&decoder, data);
		v = _jit_varint_decode_uint(&decoder);
		if(v != n)
		{
			fprintf(stderr, "FAILED\n");
		}
		v = _jit_varint_decode_uint(&decoder);
		if(v != n - 1)
		{
			fprintf(stderr, "FAILED\n");
		}
		v = _jit_varint_decode_uint(&decoder);
		if(v != n + 1)
		{
			fprintf(stderr, "FAILED\n");
		}
		_jit_varint_decode_uint(&decoder);
		if(!_jit_varint_decode_end(&decoder))
		{
			fprintf(stderr, "FAILED\n");
		}

		_jit_varint_free_data(data);
	}

}

#endif
