#ifndef SDES_X_
#define SDES_X_

EXTERN S16 Base64Encode(U8 *inBuf, U32 inLen, U8 *outBuf);
EXTERN S16 Base64Decode(U8 *inBuf, U32 inLen, U8 *outBuf);
EXTERN S16 CreateCryptoKeySalt(U8 *base64KeySalt);

#endif /*SDES_X_*/
