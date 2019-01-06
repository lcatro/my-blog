Sha 256 算法原理(代码来自Bfgminer Sha2.c):



```c
void sha256(const unsigned char *message, unsigned int len, unsigned char *digest)  //  计算message 的sha256 hash
{
    sha256_ctx ctx;

    sha256_init(&ctx);  //  初始化sha256 hash 因子
    sha256_update(&ctx, message, len);
    sha256_final(&ctx, digest);
}
  
void sha256_update(sha256_ctx *ctx, const unsigned char *message,unsigned int len)
{
    unsigned int block_nb;
    unsigned int new_len, rem_len, tmp_len;
    const unsigned char *shifted_message;

    tmp_len = SHA256_BLOCK_SIZE - ctx->len;  //  tmp_len = SHA256_BLOCK_SIZE
    rem_len = len < tmp_len ? len : tmp_len;  //  计算Sha256 摘要块大小

    memcpy(&ctx->block[ctx->len], message, rem_len);  //  复制要hash 的信息到block

    if (ctx->len + len < SHA256_BLOCK_SIZE) {
        ctx->len += len;
        return;
    }

    new_len = len - rem_len;  //  剩下的长度
    block_nb = new_len / SHA256_BLOCK_SIZE;  //  一个Sha256 Hash 运算块大小为64 * 4 Byte ,计算一共有多少个Sha256 块

    shifted_message = message + rem_len;  //  后面的运算块
  //  并行1.
    sha256_transf(ctx, ctx->block, 1);  //  先对第一块进行Sha 256 hash 运算
  //  并行2.
    sha256_transf(ctx, shifted_message, block_nb);  //  对后面的运算块进行运算
  //  并行3.
    rem_len = new_len % SHA256_BLOCK_SIZE;  //  最后没有对齐64 * 4 Byte 的数据
    memcpy(ctx->block, &shifted_message[block_nb << 6],
           rem_len);  //  没有对齐的数据

    ctx->len = rem_len;
    ctx->tot_len += (block_nb + 1) << 6;
}

void sha256_transf(sha256_ctx *ctx, const unsigned char *message,
                   unsigned int block_nb)
{
    uint32_t w[64];
    uint32_t wv[8];
    uint32_t t1, t2;
    const unsigned char *sub_block;
    int i;
    int j;

    for (i = 0; i < (int) block_nb; i++) {  //  对原消息进行分块hash 计算,由于结果互不影响,这里可以做并行计算
        sub_block = message + (i << 6);
  
        for (j = 0; j < 16; j++) {
            PACK32(&sub_block[j << 2], &w[j]);
/*
            #define PACK32(str, x)                        \
            {                                             \
                *(x) =   ((uint32_t) *((str) + 3)      )    \
                       | ((uint32_t) *((str) + 2) <<  8)    \
                       | ((uint32_t) *((str) + 1) << 16)    \
                       | ((uint32_t) *((str) + 0) << 24);   \
            }
*/
        }
        for (j = 16; j < 64; j++) {
            SHA256_SCR(j);
/*
            #define SHA256_SCR(i)                         \
            {                                             \
                w[i] =  SHA256_F4(w[i -  2]) + w[i -  7]  \
                      \+ SHA256_F3(w[i - 15]) + w[i - 16]; \
            }
            #define SHA256_F3(x) (ROTR(x,  7) ^ ROTR(x, 18) ^ SHFR(x,  3))
            #define SHA256_F4(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHFR(x, 10))
            #define SHFR(x, n)    (x >> n)
*/
        }

        for (j = 0; j < 8; j++) {
            wv[j] = ctx->h[j];
        }

        for (j = 0; j < 64; j++) {
            t1 = wv[7] + SHA256_F2(wv[4]) + CH(wv[4], wv[5], wv[6]) + sha256_k[j] + w[j];
//          #define SHA256_F2(x) (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))
//          #define ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
//          #define CH(x, y, z)  ((x & y) ^ (~x & z))
            t2 = SHA256_F1(wv[0]) + MAJ(wv[0], wv[1], wv[2]);
//          #define SHA256_F1(x) (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
//          #define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
            wv[7] = wv[6];
            wv[6] = wv[5];
            wv[5] = wv[4];
            wv[4] = wv[3] + t1;
            wv[3] = wv[2];
            wv[2] = wv[1];
            wv[1] = wv[0];
            wv[0] = t1 + t2;
        }

        for (j = 0; j < 8; j++) {
            ctx->h[j] += wv[j];
        }
    }
}

void sha256_final(sha256_ctx *ctx, unsigned char *digest)
{
    unsigned int block_nb;
    unsigned int pm_len;
    unsigned int len_b;
    int i;

    block_nb = (1 + ((SHA256_BLOCK_SIZE - 9) < (ctx->len % SHA256_BLOCK_SIZE)));

    len_b = (ctx->tot_len + ctx->len) << 3;
    pm_len = block_nb << 6;

    memset(ctx->block + ctx->len, 0, pm_len - ctx->len);
    ctx->block[ctx->len] = 0x80;
    UNPACK32(len_b, ctx->block + pm_len - 4);

    sha256_transf(ctx, ctx->block, block_nb);

    for (i = 0 ; i < 8; i++) {
        UNPACK32(ctx->h[i], &digest[i << 2]);
    }
}
```





Scrypt 算法原理(代码来自Bfgminer Malgo/Scrypt.c):



```c
/* cpu and memory intensive function to transform a 80 byte buffer into a 32 byte output
   scratchpad size needs to be at least 63 + (128 * r * p) + (256 * r + 64) + (128 * r * N) bytes
 */
static void scrypt_1024_1_1_256_sp(const uint32_t* input, char* scratchpad, uint32_t *ostate)
{
	uint32_t * V;
	uint32_t X[32];
	uint32_t i;
	uint32_t j;
	uint32_t k;
	uint64_t *p1, *p2;
	
	p1 = (uint64_t *)X;
	V = (uint32_t *)(((uintptr_t)(scratchpad) + 63) & ~ (uintptr_t)(63));
	
	PBKDF2_SHA256_80_128(input, X);  //  对input 做一次PBKDF_SHA256 hash 计算,得出X
	
	for (i = 0; i < 1024; i += 2) {  //  把PBKDF2 SHA256 的结果不断运算填充到V 中,总计大小是128 * 1024 KByte
		memcpy(&V[i * 32], X, 128);  //  X 是上面的input 计算PBKDF2_SHA256 的结果
		salsa20_8(&X[0], &X[16]);  //  16 * 4 Byte 混合位运算
		salsa20_8(&X[16], &X[0]);
		
		memcpy(&V[(i + 1) * 32], X, 128);  //  在这里,X 是必须要使用上一次的salsa20_8() 计算得出的结果再来进行运算
		salsa20_8(&X[0], &X[16]);
		salsa20_8(&X[16], &X[0]);
	}
	
	for (i = 0; i < 1024; i += 2) {  //  再次对摘要块里的数据进行位计算,由于X 是依赖上一次计算的,所以还是无法做到并行计算
		j = X[16] & 1023;
		p2 = (uint64_t *)(&V[j * 32]);
		for(k = 0; k < 16; k++)  //  用之前的V 块里边的数据再和X 进行异或计算
			p1[k] ^= p2[k];  //  p1 是X 的指针,所以p1 就是X
		
		salsa20_8(&X[0], &X[16]);
		salsa20_8(&X[16], &X[0]);
		
		j = X[16] & 1023;
		p2 = (uint64_t *)(&V[j * 32]);
		for(k = 0; k < 16; k++)
			p1[k] ^= p2[k];
		
		salsa20_8(&X[0], &X[16]);
		salsa20_8(&X[16], &X[0]);
	}
	//  上面的逻辑主要是根据input 算PBKDF2 的加盐因子,给下面的SHA256 算法提供加盐因子
	PBKDF2_SHA256_80_128_32(input, X, ostate);  //  以X 为加盐因子,计算input 的PBKDF2_SHA256 hash
}

/**
 \* salsa20_8(B):
 \* Apply the salsa20/8 core to the provided block.
 */
static inline void salsa20_8(uint32_t B[16], const uint32_t Bx[16])
{
	uint32_t x00,x01,x02,x03,x04,x05,x06,x07,x08,x09,x10,x11,x12,x13,x14,x15;
	size_t i;
	
	x00 = (B[ 0] ^= Bx[ 0]);
	x01 = (B[ 1] ^= Bx[ 1]);
	x02 = (B[ 2] ^= Bx[ 2]);
	x03 = (B[ 3] ^= Bx[ 3]);
	x04 = (B[ 4] ^= Bx[ 4]);
	x05 = (B[ 5] ^= Bx[ 5]);
	x06 = (B[ 6] ^= Bx[ 6]);
	x07 = (B[ 7] ^= Bx[ 7]);
	x08 = (B[ 8] ^= Bx[ 8]);
	x09 = (B[ 9] ^= Bx[ 9]);
	x10 = (B[10] ^= Bx[10]);
	x11 = (B[11] ^= Bx[11]);
	x12 = (B[12] ^= Bx[12]);
	x13 = (B[13] ^= Bx[13]);
	x14 = (B[14] ^= Bx[14]);
	x15 = (B[15] ^= Bx[15]);
	for (i = 0; i < 8; i += 2) {
#define R(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
		/* Operate on columns. */
		x04 ^= R(x00+x12, 7);	x09 ^= R(x05+x01, 7);	x14 ^= R(x10+x06, 7);	x03 ^= R(x15+x11, 7);
		x08 ^= R(x04+x00, 9);	x13 ^= R(x09+x05, 9);	x02 ^= R(x14+x10, 9);	x07 ^= R(x03+x15, 9);
		x12 ^= R(x08+x04,13);	x01 ^= R(x13+x09,13);	x06 ^= R(x02+x14,13);	x11 ^= R(x07+x03,13);
		x00 ^= R(x12+x08,18);	x05 ^= R(x01+x13,18);	x10 ^= R(x06+x02,18);	x15 ^= R(x11+x07,18);
	
		/* Operate on rows. */
		x01 ^= R(x00+x03, 7);	x06 ^= R(x05+x04, 7);	x11 ^= R(x10+x09, 7);	x12 ^= R(x15+x14, 7);
		x02 ^= R(x01+x00, 9);	x07 ^= R(x06+x05, 9);	x08 ^= R(x11+x10, 9);	x13 ^= R(x12+x15, 9);
		x03 ^= R(x02+x01,13);	x04 ^= R(x07+x06,13);	x09 ^= R(x08+x11,13);	x14 ^= R(x13+x12,13);
		x00 ^= R(x03+x02,18);	x05 ^= R(x04+x07,18);	x10 ^= R(x09+x08,18);	x15 ^= R(x14+x13,18);
#undef R
	}
	B[ 0] += x00;
	B[ 1] += x01;
	B[ 2] += x02;
	B[ 3] += x03;
	B[ 4] += x04;
	B[ 5] += x05;
	B[ 6] += x06;
	B[ 7] += x07;
	B[ 8] += x08;
	B[ 9] += x09;
	B[10] += x10;
	B[11] += x11;
	B[12] += x12;
	B[13] += x13;
	B[14] += x14;
	B[15] += x15;
}
```





PBKDF2 SHA256 算法



```c
static inline void
PBKDF2_SHA256_80_128_32(const uint32_t * passwd, const uint32_t * salt, uint32_t *ostate)
{
	uint32_t tstate[8];
	uint32_t ihash[8];
	uint32_t i;
	
	/* Compute HMAC state after processing P and S. */
	uint32_t pad[16];
	
	static const uint32_t ihash_finalblk[16] = {0x00000001,0x80000000,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0x00000620};
	
	/* If Klen > 64, the key is really SHA256(K). */
	SHA256_InitState(tstate);  //  初始化tstate
	SHA256_Transform(tstate, passwd, 1);  //  tstate 和passwd 一起算SHA256
	memcpy(pad, passwd+16, 16);
	memcpy(pad+4, passwdpad, 48);
	SHA256_Transform(tstate, pad, 1);  //  tstate 和pad 一起算SHA256
	memcpy(ihash, tstate, 32);  //  tstate 保存到ihash,其实这里可以做优化的,直接对ihash 计算就行了
	
	SHA256_InitState(ostate);  //  初始化ostate
	for (i = 0; i < 8; i++)
		pad[i] = ihash[i] ^ 0x5c5c5c5c;
	for (; i < 16; i++)
		pad[i] = 0x5c5c5c5c;
	SHA256_Transform(ostate, pad, 0);  //  ostate 和pad 一起算SHA256
	
	SHA256_InitState(tstate);  //  初始化tstate
	for (i = 0; i < 8; i++)
		pad[i] = ihash[i] ^ 0x36363636;
	for (; i < 16; i++)
		pad[i] = 0x36363636;
	SHA256_Transform(tstate, pad, 0);  //  tstate 和pad 一起算SHA256
	SHA256_Transform(tstate, salt, 1);  //  tstate 和salt 一起算SHA256
	SHA256_Transform(tstate, salt+16, 1);  //  tstate 和salt[16] 一起算SHA256
	SHA256_Transform(tstate, ihash_finalblk, 0);  //  tstate 和ihash_finalblk 一起算SHA256
	memcpy(pad, tstate, 32);
	memcpy(pad+8, outerpad, 32);
	
	/* Feed the inner hash to the outer SHA256 operation. */
	SHA256_Transform(ostate, pad, 0);
}




/**
 \* PBKDF2_SHA256(passwd, passwdlen, salt, saltlen, c, buf, dkLen):
 \* Compute PBKDF2(passwd, salt, c, dkLen) using HMAC-SHA256 as the PRF, and
 \* write the output to buf.  The value dkLen must be at most 32 * (2^32 - 1).
 */
static inline void
PBKDF2_SHA256_80_128(const uint32_t * passwd, uint32_t * buf)
{
	SHA256_CTX PShictx, PShoctx;
	uint32_t tstate[8];
	uint32_t ihash[8];
	uint32_t i;
	uint32_t pad[16];
	
	static const uint32_t innerpad[11] = {0x00000080, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xa0040000};

	/* If Klen > 64, the key is really SHA256(K). */
	SHA256_InitState(tstate);
	SHA256_Transform(tstate, passwd, 1);
	memcpy(pad, passwd+16, 16);
	memcpy(pad+4, passwdpad, 48);
	SHA256_Transform(tstate, pad, 1);
	memcpy(ihash, tstate, 32);

	SHA256_InitState(PShictx.state);
	for (i = 0; i < 8; i++)
		pad[i] = ihash[i] ^ 0x36363636;
	for (; i < 16; i++)
		pad[i] = 0x36363636;
	SHA256_Transform(PShictx.state, pad, 0);
	SHA256_Transform(PShictx.state, passwd, 1);
	be32enc_vect(PShictx.buf, passwd+16, 4);
	be32enc_vect(PShictx.buf+5, innerpad, 11);

	SHA256_InitState(PShoctx.state);
	for (i = 0; i < 8; i++)
		pad[i] = ihash[i] ^ 0x5c5c5c5c;
	for (; i < 16; i++)
		pad[i] = 0x5c5c5c5c;
	SHA256_Transform(PShoctx.state, pad, 0);
	memcpy(PShoctx.buf+8, outerpad, 32);

	/* Iterate through the blocks. */
	swap32tole(PShictx.buf, PShictx.buf, 0x10);
	for (i = 0; i < 4; i++) {
		uint32_t istate[8];
		uint32_t ostate[8];
		
		memcpy(istate, PShictx.state, 32);
		PShictx.buf[4] = i + 1;
		SHA256_Transform(istate, PShictx.buf, 0);
		memcpy(PShoctx.buf, istate, 32);

		memcpy(ostate, PShoctx.state, 32);
		SHA256_Transform(ostate, PShoctx.buf, 0);
		swap32yes(buf+i*8, ostate, 8);
	}
}
```

