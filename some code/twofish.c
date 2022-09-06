#include <string.h>
#include "twofish.h"
#define Twofish_fatal( msg )      {for(;;);}
#define UINT32_MASK    ( (((UInt32)2)<<31) - 1 )
#define ROL32( x, n )  ( (x)<<(n) | ((x) & UINT32_MASK) >> (32-(n)) )
#define ROR32( x, n )  ROL32( (x), 32-(n) )
#define LARGE_Q_TABLE   0    
#define SELECT_BYTE_FROM_UINT32_IN_MEMORY    0    
#define CONVERT_USING_CASTS    0    
#define CPU_IS_BIG_ENDIAN    0
#define BSWAP(x) ((ROL32((x),8) & 0x00ff00ff) | (ROR32((x),8) & 0xff00ff00))
typedef Twofish_Byte    Byte;
typedef Twofish_UInt32  UInt32;
#if CPU_IS_BIG_ENDIAN
#define ENDIAN_CONVERT(x)    BSWAP(x)
#else
#define ENDIAN_CONVERT(x)    (x)
#endif
#if CPU_IS_BIG_ENDIAN
#define BYTE_OFFSET( n )  (sizeof(UInt32) - 1 - (n) )
#else
#define BYTE_OFFSET( n )  (n)
#endif
#if SELECT_BYTE_FROM_UINT32_IN_MEMORY
#define SELECT_BYTE( X, b ) (((Byte *)(&(X)))[BYTE_OFFSET(b)])
#else
#define SELECT_BYTE( X, b ) (((X) >> 8*(b)) & 0xff)
#endif
#define b0(X)   SELECT_BYTE((X),0)
#define b1(X)   SELECT_BYTE((X),1)
#define b2(X)   SELECT_BYTE((X),2)
#define b3(X)   SELECT_BYTE((X),3)
#if CONVERT_USING_CASTS
#define GET32( p )    ENDIAN_CONVERT( *((UInt32 *)(p)) )
#define PUT32( v, p ) *((UInt32 *)(p)) = ENDIAN_CONVERT(v)
#else
#define GET32( p ) \
( \
(UInt32)((p)[0])    \
| (UInt32)((p)[1])<< 8\
| (UInt32)((p)[2])<<16\
| (UInt32)((p)[3])<<24\
)
#define PUT32( v, p ) \
(p)[0] = (Byte)(((v)      ) & 0xff);\
(p)[1] = (Byte)(((v) >>  8) & 0xff);\
(p)[2] = (Byte)(((v) >> 16) & 0xff);\
(p)[3] = (Byte)(((v) >> 24) & 0xff)
#endif
static void test_platform(void)
{
  Byte buf[] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0};
  UInt32 C;
  UInt32 x,y;
  int i;
  if( ((UInt32) ((UInt32)1 << 31) == 0) || ((UInt32)-1 < 0) )
  {
    Twofish_fatal( "Twofish code: Twofish_UInt32 type not suitable" );
  }
  if( (sizeof( Byte ) != 1) || ((Byte)-1 < 0) )
  {
    Twofish_fatal( "Twofish code: Twofish_Byte type not suitable" )
  }
  if( GET32( buf ) != 0x78563412UL || GET32(buf+1) != 0x9a785634UL
     || GET32( buf+2 ) != 0xbc9a7856UL || GET32(buf+3) != 0xdebc9a78UL )
  {
    Twofish_fatal( "Twofish code: GET32 not implemented properly" );
  }
  C = GET32( buf );
  PUT32( 3*C, buf );
  if( GET32( buf ) != 0x69029c36UL )
  {
    Twofish_fatal( "Twofish code: PUT32 not implemented properly" );
  }
  for( i=1; i<32; i++ )
  {
    x = ROR32( C, i );
    y = ROL32( C, i );
    x ^= (C>>i) ^ (C<<(32-i));
    y ^= (C<<i) ^ (C>>(32-i));
    x |= y;
    if( (x & 0xffffffffUL) != 0 )
    {
      Twofish_fatal( "Twofish ROL or ROR not properly defined." );
    }
  }
  if( (BSWAP(C)) != 0x12345678UL )
  {
    Twofish_fatal( "BSWAP not properly defined." );
  }
  if( (b0(C)!=0x12) || (b1(C) != 0x34) || (b2(C) != 0x56) || (b3(C) != 0x78) )
  {
    Twofish_fatal( "Twofish code: SELECT_BYTE not implemented properly" );
  }
}
static void test_vector( Byte key[], int key_len, Byte p[16], Byte c[16] )
{
  Byte tmp[16];               
  Twofish_key xkey;           
  int i;
  Twofish_prepare_key( key, key_len, &xkey );
  for( i=0; i<2; i++ )
  {
    Twofish_encrypt_block( &xkey, p, tmp );
    if( memcmp( c, tmp, 16 ) != 0 )
    {
      Twofish_fatal( "Twofish encryption failure" );
    }
    Twofish_decrypt_block( &xkey, c, tmp );
    if( memcmp( p, tmp, 16 ) != 0 )
    {
      Twofish_fatal( "Twofish decryption failure" );
    }
  }
}
static void test_vectors()
{
  static Byte k128[] = {
    0x9F, 0x58, 0x9F, 0x5C, 0xF6, 0x12, 0x2C, 0x32,
    0xB6, 0xBF, 0xEC, 0x2F, 0x2A, 0xE8, 0xC3, 0x5A,
  };
  static Byte p128[] = {
    0xD4, 0x91, 0xDB, 0x16, 0xE7, 0xB1, 0xC3, 0x9E,
    0x86, 0xCB, 0x08, 0x6B, 0x78, 0x9F, 0x54, 0x19
  };
  static Byte c128[] = {
    0x01, 0x9F, 0x98, 0x09, 0xDE, 0x17, 0x11, 0x85,
    0x8F, 0xAA, 0xC3, 0xA3, 0xBA, 0x20, 0xFB, 0xC3
  };
  static Byte k192[] = {
    0x88, 0xB2, 0xB2, 0x70, 0x6B, 0x10, 0x5E, 0x36,
    0xB4, 0x46, 0xBB, 0x6D, 0x73, 0x1A, 0x1E, 0x88,
    0xEF, 0xA7, 0x1F, 0x78, 0x89, 0x65, 0xBD, 0x44
  };
  static Byte p192[] = {
    0x39, 0xDA, 0x69, 0xD6, 0xBA, 0x49, 0x97, 0xD5,
    0x85, 0xB6, 0xDC, 0x07, 0x3C, 0xA3, 0x41, 0xB2
  };
  static Byte c192[] = {
    0x18, 0x2B, 0x02, 0xD8, 0x14, 0x97, 0xEA, 0x45,
    0xF9, 0xDA, 0xAC, 0xDC, 0x29, 0x19, 0x3A, 0x65
  };
  static Byte k256[] = {
    0xD4, 0x3B, 0xB7, 0x55, 0x6E, 0xA3, 0x2E, 0x46,
    0xF2, 0xA2, 0x82, 0xB7, 0xD4, 0x5B, 0x4E, 0x0D,
    0x57, 0xFF, 0x73, 0x9D, 0x4D, 0xC9, 0x2C, 0x1B,
    0xD7, 0xFC, 0x01, 0x70, 0x0C, 0xC8, 0x21, 0x6F
  };
  static Byte p256[] = {
    0x90, 0xAF, 0xE9, 0x1B, 0xB2, 0x88, 0x54, 0x4F,
    0x2C, 0x32, 0xDC, 0x23, 0x9B, 0x26, 0x35, 0xE6
  };
  static Byte c256[] = {
    0x6C, 0xB4, 0x56, 0x1C, 0x40, 0xBF, 0x0A, 0x97,
    0x05, 0x93, 0x1C, 0xB6, 0xD4, 0x08, 0xE7, 0xFA
  };
  test_vector( k128, 16, p128, c128 );
  test_vector( k192, 24, p192, c192 );
  test_vector( k256, 32, p256, c256 );
}
static void test_sequence( int key_len, Byte final_value[] )
{
  Byte buf[ (50+3)*16 ];      
  Byte tmp[16];               
  Twofish_key xkey;           
  int i;
  Byte * p;
  memset( buf, 0, sizeof( buf ) );
  p = &buf[50*16];
  for( i=1; i<50; i++ )
  {
    Twofish_prepare_key( p+16, key_len, &xkey );
    Twofish_encrypt_block( &xkey, p, p-16 );
    Twofish_decrypt_block( &xkey, p-16, tmp );
    if( memcmp( tmp, p, 16 ) != 0 )
    {
      Twofish_fatal( "Twofish decryption failure in sequence" );
    }
    p -= 16;
  }
  if( memcmp( p, final_value, 16 ) != 0 )
  {
    Twofish_fatal( "Twofish encryption failure in sequence" );
  }
}
static void test_sequences()
{
  static Byte r128[] = {
    0x5D, 0x9D, 0x4E, 0xEF, 0xFA, 0x91, 0x51, 0x57,
    0x55, 0x24, 0xF1, 0x15, 0x81, 0x5A, 0x12, 0xE0
  };
  static Byte r192[] = {
    0xE7, 0x54, 0x49, 0x21, 0x2B, 0xEE, 0xF9, 0xF4,
    0xA3, 0x90, 0xBD, 0x86, 0x0A, 0x64, 0x09, 0x41
  };
  static Byte r256[] = {
    0x37, 0xFE, 0x26, 0xFF, 0x1C, 0xF6, 0x61, 0x75,
    0xF5, 0xDD, 0xF4, 0xC3, 0x3B, 0x97, 0xA2, 0x05
  };
  test_sequence( 16, r128 );
  test_sequence( 24, r192 );
  test_sequence( 32, r256 );
}
static void test_odd_sized_keys()
{
  Byte buf[32];
  Twofish_key xkey;
  Twofish_key xkey_two;
  int i;
  memset( buf, 0, sizeof( buf ) );
  Twofish_prepare_key( buf, 16, &xkey );
  Twofish_encrypt_block( &xkey, buf, buf );
  Twofish_encrypt_block( &xkey, buf, buf+16 );
  for( i=31; i>=0; i-- )
  {
    buf[i] = 0;
    Twofish_prepare_key( buf, i, &xkey );
    Twofish_prepare_key( buf, i<=16 ? 16 : i<= 24 ? 24 : 32, &xkey_two );
    if( memcmp( &xkey, &xkey_two, sizeof( xkey ) ) != 0 )
    {
      Twofish_fatal( "Odd sized keys do not expand properly" );
    }
  }
}
static void self_test()
{
  test_vectors();
  test_sequences();
  test_odd_sized_keys();
}
static Byte t_table[2][4][16] = {
  {
    {0x8,0x1,0x7,0xD,0x6,0xF,0x3,0x2,0x0,0xB,0x5,0x9,0xE,0xC,0xA,0x4},
    {0xE,0xC,0xB,0x8,0x1,0x2,0x3,0x5,0xF,0x4,0xA,0x6,0x7,0x0,0x9,0xD},
    {0xB,0xA,0x5,0xE,0x6,0xD,0x9,0x0,0xC,0x8,0xF,0x3,0x2,0x4,0x7,0x1},
    {0xD,0x7,0xF,0x4,0x1,0x2,0x6,0xE,0x9,0xB,0x3,0x0,0x8,0x5,0xC,0xA}
  },
  {
    {0x2,0x8,0xB,0xD,0xF,0x7,0x6,0xE,0x3,0x1,0x9,0x4,0x0,0xA,0xC,0x5},
    {0x1,0xE,0x2,0xB,0x4,0xC,0x3,0x7,0x6,0xD,0xA,0x5,0xF,0x9,0x0,0x8},
    {0x4,0xC,0x7,0x5,0x1,0x6,0x9,0xA,0x0,0xE,0xD,0x8,0x2,0xB,0x3,0xF},
    {0xB,0x9,0x5,0x1,0xC,0x3,0xD,0xE,0x6,0x4,0x7,0xF,0x2,0x0,0x8,0xA}
  }
};
#define ROR4BY1( x ) (((x)>>1) | (((x)<<3) & 0x8) )
#if LARGE_Q_TABLE
typedef UInt32      Qtype;
#else
typedef Byte        Qtype;
#endif
static Qtype q_table[2][256];
static void make_q_table( Byte t[4][16], Qtype q[256] )
{
  int ae,be,ao,bo;        
  int i;
  for( i=0; i<256; i++ ) {
    ae = i>>4; be = i&0xf;
    ao = ae ^ be; bo = ae ^ ROR4BY1(be) ^ ((ae<<3)&8);
    ae = t[0][ao]; be = t[1][bo];
    ao = ae ^ be; bo = ae ^ ROR4BY1(be) ^ ((ae<<3)&8);
    ae = t[2][ao]; be = t[3][bo];
    q[i] = (Qtype) ((be<<4) | ae);
  }
}
static void initialise_q_boxes() {
  make_q_table( t_table[0], q_table[0] );
  make_q_table( t_table[1], q_table[1] );
}
static UInt32 MDS_table[4][256];
static UInt32 mds_poly_divx_const[] = {0,0xb4};
static void initialise_mds_tables()
{
  int i;
  UInt32 q,qef,q5b;       
  for( i=0; i<256; i++ )
  {
    q = q_table[0][i];
    qef = (q >> 1) ^ mds_poly_divx_const[ q & 1 ];
    q5b = (qef >> 1) ^ mds_poly_divx_const[ qef & 1 ] ^ q;
    qef ^= q5b;
    MDS_table[1][i] = q  <<24 | q5b<<16 | qef<<8 | qef;
    MDS_table[3][i] = q5b<<24 | qef<<16 | q  <<8 | q5b;
    q = q_table[1][i];
    qef = (q >> 1) ^ mds_poly_divx_const[ q & 1 ];
    q5b = (qef >> 1) ^ mds_poly_divx_const[ qef & 1 ] ^ q;
    qef ^= q5b;
    MDS_table[0][i] = qef<<24 | qef<<16 | q5b<<8 | q  ;
    MDS_table[2][i] = qef<<24 | q  <<16 | qef<<8 | q5b;
  }
}
#define q0  q_table[0]
#define q1  q_table[1]
#define H02( y, L )  MDS_table[0][q0[q0[y]^L[ 8]]^L[0]]
#define H12( y, L )  MDS_table[1][q0[q1[y]^L[ 9]]^L[1]]
#define H22( y, L )  MDS_table[2][q1[q0[y]^L[10]]^L[2]]
#define H32( y, L )  MDS_table[3][q1[q1[y]^L[11]]^L[3]]
#define H03( y, L )  H02( q1[y]^L[16], L )
#define H13( y, L )  H12( q1[y]^L[17], L )
#define H23( y, L )  H22( q0[y]^L[18], L )
#define H33( y, L )  H32( q0[y]^L[19], L )
#define H04( y, L )  H03( q1[y]^L[24], L )
#define H14( y, L )  H13( q0[y]^L[25], L )
#define H24( y, L )  H23( q0[y]^L[26], L )
#define H34( y, L )  H33( q1[y]^L[27], L )
static UInt32 h( int k, Byte L[], int kCycles )
{
  switch( kCycles ) {
    case 2:
      return H02(k,L) ^ H12(k,L) ^ H22(k,L) ^ H32(k,L);
    case 3:
      return H03(k,L) ^ H13(k,L) ^ H23(k,L) ^ H33(k,L);
    case 4:
      return H04(k,L) ^ H14(k,L) ^ H24(k,L) ^ H34(k,L);
    default:
      Twofish_fatal( "Twofish h(): Illegal argument" );
  }
}
static void fill_keyed_sboxes( Byte S[], int kCycles, Twofish_key * xkey )
{
  int i;
  switch( kCycles ) {
    case 2:
      for( i=0; i<256; i++ )
      {
        xkey->s[0][i]= H02( i, S );
        xkey->s[1][i]= H12( i, S );
        xkey->s[2][i]= H22( i, S );
        xkey->s[3][i]= H32( i, S );
      }
      break;
    case 3:
      for( i=0; i<256; i++ )
      {
        xkey->s[0][i]= H03( i, S );
        xkey->s[1][i]= H13( i, S );
        xkey->s[2][i]= H23( i, S );
        xkey->s[3][i]= H33( i, S );
      }
      break;
    case 4:
      for( i=0; i<256; i++ )
      {
        xkey->s[0][i]= H04( i, S );
        xkey->s[1][i]= H14( i, S );
        xkey->s[2][i]= H24( i, S );
        xkey->s[3][i]= H34( i, S );
      }
      break;
    default:
      Twofish_fatal( "Twofish fill_keyed_sboxes(): Illegal argument" );
  }
}
static int Twofish_initialised = 0;
void Twofish_initialise(void)
{
  test_platform();
  initialise_q_boxes();
  initialise_mds_tables();
  Twofish_initialised = 1;
  self_test();
}
static unsigned int rs_poly_const[] = {0, 0x14d};
static unsigned int rs_poly_div_const[] = {0, 0xa6 };
void Twofish_setup(Twofish_context *context, Twofish_Byte key[32], Twofish_Byte iv[16], Twofish_options options) {
  Twofish_initialise();
  context->options = options;
  Twofish_prepare_key(key, 32, &(context->key));
  memcpy(context->iv, iv, 16);
}
void Twofish_prepare_key( Byte key[], int key_len, Twofish_key * xkey )
{
  Byte K[32+32+4];
  int kCycles;        
  int i;
  UInt32 A, B;        
  Byte * kptr;        
  Byte * sptr;
  Byte * t;
  Byte b,bx,bxx;      
  if( Twofish_initialised == 0 )
  {
    Twofish_fatal( "Twofish implementation was not initialised." );
    return;
  }
  if( key_len < 0 || key_len > 32 )
  {
    Twofish_fatal( "Twofish_prepare_key: illegal key length" );
    return;
  }
  memcpy( K, key, key_len );
  memset( K+key_len, 0, sizeof(K)-key_len );
  kCycles = (key_len + 7) >> 3;
  if( kCycles < 2 )
  {
    kCycles = 2;
  }
  for( i=0; i<40; i+=2 )
  {
    A = h( i  , K  , kCycles );
    B = h( i+1, K+4, kCycles );
    B = ROL32( B, 8 );
    A += B;
    B += A;
    xkey->K[i]   = A;
    xkey->K[i+1] = ROL32( B, 9 );
  }
  A=B=0;
  kptr = K + 8*kCycles;           
  sptr = K + 32;                  
  while( kptr > K )
  {
    kptr -= 8;
    memset( sptr, 0, 4 );
    memcpy( sptr+4, kptr, 8 );
    t = sptr+11;
    while( t > sptr+3 )
    {
      b = *t;
      bx = (Byte)((b<<1) ^ rs_poly_const[ b>>7 ]);
      bxx= (Byte)((b>>1) ^ rs_poly_div_const[ b&1 ] ^ bx);
      t[-1] ^= bxx;
      t[-2] ^= bx;
      t[-3] ^= bxx;
      t[-4] ^= b;
      t--;
    }
    sptr += 8;
  }
  b = bx = bxx = 0;
  fill_keyed_sboxes( &K[32], kCycles, xkey );
  memset( K, 0, sizeof( K ) );
}
#define g0(X,xkey) \
(xkey->s[0][b0(X)]^xkey->s[1][b1(X)]^xkey->s[2][b2(X)]^xkey->s[3][b3(X)])
#define g1(X,xkey) \
(xkey->s[0][b3(X)]^xkey->s[1][b0(X)]^xkey->s[2][b1(X)]^xkey->s[3][b2(X)])
#define ENCRYPT_RND( A,B,C,D, T0, T1, xkey, r ) \
T0 = g0(A,xkey); T1 = g1(B,xkey);\
C ^= T0+T1+xkey->K[8+2*(r)]; C = ROR32(C,1);\
D = ROL32(D,1); D ^= T0+2*T1+xkey->K[8+2*(r)+1]
#define ENCRYPT_CYCLE( A, B, C, D, T0, T1, xkey, r ) \
ENCRYPT_RND( A,B,C,D,T0,T1,xkey,2*(r)   );\
ENCRYPT_RND( C,D,A,B,T0,T1,xkey,2*(r)+1 )
#define ENCRYPT( A,B,C,D,T0,T1,xkey ) \
ENCRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 0 );\
ENCRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 1 );\
ENCRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 2 );\
ENCRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 3 );\
ENCRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 4 );\
ENCRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 5 );\
ENCRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 6 );\
ENCRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 7 )
#define DECRYPT_RND( A,B,C,D, T0, T1, xkey, r ) \
T0 = g0(A,xkey); T1 = g1(B,xkey);\
C = ROL32(C,1); C ^= T0+T1+xkey->K[8+2*(r)];\
D ^= T0+2*T1+xkey->K[8+2*(r)+1]; D = ROR32(D,1)
#define DECRYPT_CYCLE( A, B, C, D, T0, T1, xkey, r ) \
DECRYPT_RND( A,B,C,D,T0,T1,xkey,2*(r)+1 );\
DECRYPT_RND( C,D,A,B,T0,T1,xkey,2*(r)   )
#define DECRYPT( A,B,C,D,T0,T1, xkey ) \
DECRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 7 );\
DECRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 6 );\
DECRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 5 );\
DECRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 4 );\
DECRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 3 );\
DECRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 2 );\
DECRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 1 );\
DECRYPT_CYCLE( A,B,C,D,T0,T1,xkey, 0 )
#define GET_INPUT( src, A,B,C,D, xkey, koff ) \
A = GET32(src   )^xkey->K[  koff]; B = GET32(src+ 4)^xkey->K[1+koff]; \
C = GET32(src+ 8)^xkey->K[2+koff]; D = GET32(src+12)^xkey->K[3+koff]
#define PUT_OUTPUT( A,B,C,D, dst, xkey, koff ) \
A ^= xkey->K[  koff]; B ^= xkey->K[1+koff]; \
C ^= xkey->K[2+koff]; D ^= xkey->K[3+koff]; \
PUT32( A, dst   ); PUT32( B, dst+ 4 ); \
PUT32( C, dst+8 ); PUT32( D, dst+12 )
void Twofish_encrypt_block( Twofish_key * xkey, Byte p[16], Byte c[16]) {
  UInt32 A,B,C,D,T0,T1;       
  GET_INPUT( p, A,B,C,D, xkey, 0 );
  ENCRYPT( A,B,C,D,T0,T1,xkey );
  PUT_OUTPUT( C,D,A,B, c, xkey, 4 );
}
void Twofish_decrypt_block( Twofish_key * xkey, Byte c[16], Byte p[16]) {
  UInt32 A,B,C,D,T0,T1;       
  GET_INPUT( c, A,B,C,D, xkey, 4 );
  DECRYPT( A,B,C,D,T0,T1,xkey );
  PUT_OUTPUT( C,D,A,B, p, xkey, 0 );
}
#define TWOFISH_BLOCKSIZE 16
Twofish_UInt64 Twofish_get_block_count(Twofish_context *context, Twofish_UInt64 input_lenght) {
  if(context->options & Twofish_option_PaddingPKCS7) {
    return 1 + (input_lenght / TWOFISH_BLOCKSIZE);
  }
  return (input_lenght / TWOFISH_BLOCKSIZE);
}
Twofish_UInt64 Twofish_get_output_length(Twofish_context *context, Twofish_UInt64 input_lenght) {
  return TWOFISH_BLOCKSIZE * Twofish_get_block_count(context, input_lenght);
}
#define TWOFISH_MIN(x,y) (x) < (y) ? (x) : (y)
void Twofish_encrypt(Twofish_context *context, Twofish_Byte *input, Twofish_UInt64 input_length, Twofish_Byte *output, Twofish_UInt64 output_length) {
  Twofish_UInt64 blockCount = Twofish_get_block_count(context, input_length);
  if(output_length < Twofish_get_output_length(context, input_length)) {
    return;
  }
  for(uint32_t blockIndex = 0; blockIndex < blockCount; blockIndex++) {
    uint8_t inputBlock[TWOFISH_BLOCKSIZE];
    uint8_t copy_length = TWOFISH_MIN(input_length - blockIndex * TWOFISH_BLOCKSIZE, TWOFISH_BLOCKSIZE);
    uint8_t paddingCount = (TWOFISH_BLOCKSIZE - copy_length);
    if(copy_length > 0) {
      memcpy(inputBlock, (input+(blockIndex*TWOFISH_BLOCKSIZE)), TWOFISH_BLOCKSIZE);
    }
    for(int index=0; index < paddingCount; index++) {
      inputBlock[TWOFISH_BLOCKSIZE - 1 - index] = paddingCount;
    }
    for(uint8_t index=0; index < TWOFISH_BLOCKSIZE; index++) {
      inputBlock[index] ^= context->iv[index];
    }
    uint8_t outputBlock[TWOFISH_BLOCKSIZE];
    Twofish_encrypt_block(&(context->key), inputBlock, outputBlock);
    memcpy(context->iv, outputBlock, TWOFISH_BLOCKSIZE);
    memcpy((output + (blockIndex * TWOFISH_BLOCKSIZE)), outputBlock, TWOFISH_BLOCKSIZE);
  }
}
void Twofish_decrypt(Twofish_context *context, Twofish_Byte *input, Twofish_UInt64 input_length, Twofish_Byte *output, Twofish_UInt64 *output_length) {
  if(NULL == output) {
    return;
  }
  if(*output_length < input_length) {
    return;
  }
  Twofish_UInt64 blockCount = input_length / TWOFISH_BLOCKSIZE;
  for(uint32_t blockIndex = 0; blockIndex < blockCount; blockIndex++) {
    uint8_t inputBlock[TWOFISH_BLOCKSIZE];
    memcpy(inputBlock, (input+(blockIndex*TWOFISH_BLOCKSIZE)), TWOFISH_BLOCKSIZE);
    uint8_t outputBlock[TWOFISH_BLOCKSIZE];
    Twofish_decrypt_block(&(context->key), inputBlock, outputBlock);
    for(uint8_t index=0; index < TWOFISH_BLOCKSIZE; index++) {
      outputBlock[index] ^= context->iv[index];
    }
    memcpy(context->iv, inputBlock, TWOFISH_BLOCKSIZE);
    memcpy((output + (blockIndex * TWOFISH_BLOCKSIZE)), outputBlock, TWOFISH_BLOCKSIZE);
  }
  *output_length = (input_length - (Twofish_Byte)output[input_length-1]);
}
