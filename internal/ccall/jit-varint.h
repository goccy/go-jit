/*
 * jit-varint.h - Variable length integer encoding.
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

#ifndef _JIT_VARINT_H
#define	_JIT_VARINT_H

#ifdef	__cplusplus
extern "C" {
#endif

#define	JIT_VARINT_BUFFER_SIZE		500

typedef struct jit_varint_data *jit_varint_data_t;
struct jit_varint_data
{
	jit_varint_data_t	next;
	unsigned char		data[];
};

typedef struct jit_varint_encoder jit_varint_encoder_t;
struct jit_varint_encoder
{
	unsigned char		buf[JIT_VARINT_BUFFER_SIZE];
	int			len;
	jit_varint_data_t	data;
	jit_varint_data_t	last;

};

typedef struct jit_varint_decoder jit_varint_decoder_t;
struct jit_varint_decoder
{
	jit_varint_data_t	data;
	int			len;
	int			end;
};

void _jit_varint_init_encoder(jit_varint_encoder_t *encoder);
void _jit_varint_init_decoder(jit_varint_decoder_t *decoder, jit_varint_data_t data);
int _jit_varint_encode_end(jit_varint_encoder_t *encoder);
int _jit_varint_encode_uint(jit_varint_encoder_t *encoder, jit_uint value);
jit_varint_data_t _jit_varint_get_data(jit_varint_encoder_t *encoder);
void _jit_varint_free_data(jit_varint_data_t data);
int _jit_varint_decode_end(jit_varint_decoder_t *decoder);
jit_uint _jit_varint_decode_uint(jit_varint_decoder_t *decoder);


#ifdef	__cplusplus
}
#endif

#endif	/* _JIT_VARINT_H */

