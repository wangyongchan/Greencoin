#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <bitset>


#include "cryptopp/eccrypto.h"
#include "cryptopp/osrng.h"
#include "cryptopp/oids.h"
#include "cryptopp/files.h"
#include "cryptopp/hex.h"

namespace crp
{
    using namespace CryptoPP;
    ByteQueue que;
    class crypto_functions
    {
    public:
        std::string pCreate()
        {
                que.Clear();
                AutoSeededRandomPool prng;
                ECDSA<ECP, SHA256>::PrivateKey k1;
                k1.Initialize( prng, ASN1::secp256r1());
                k1.Save(que);
                const Integer& x1 = k1.GetPrivateExponent();
                std::ostringstream oss1;
                oss1 << std::hex << x1;
                std::string mcreated(oss1.str());
                return mcreated;
        }
        void pLoad(const std::string& load)
        {
                que.Clear();
                ECDSA<ECP, SHA256>::PrivateKey k2;
                HexDecoder decoder1;
                decoder1.Put((byte*)&load[0], load.size());
                decoder1.MessageEnd();
                Integer x;
                x.Decode(decoder1, decoder1.MaxRetrievable());
                k2.Initialize(ASN1::secp256r1(), x);
                k2.Save(que);
        }
        std::string pSave()
        {
                ECDSA<ECP, SHA256>::PrivateKey k3;
                k3.Load(que);
                k3.Save(que);
                ECDSA<ECP, SHA256>::PublicKey pubKey1;
                k3.MakePublicKey(pubKey1);
                const ECP::Point& q = pubKey1.GetPublicElement();
                const Integer& qx = q.x;
                const Integer& qy = q.y;
                std::ostringstream oss2;
                oss2 << std::hex << qx << qy;
                std::string msaved(oss2.str());
                return msaved;
        }
        std::string pSign(const std::string& sign)
        {
                AutoSeededRandomPool prng;
                ECDSA<ECP, SHA256>::PrivateKey k4;
                k4.Load(que);
                k4.Save(que);
                ECDSA<ECP,SHA256>::Signer signer(k4);
                size_t siglen = signer.MaxSignatureLength();
                std::basic_string<unsigned char> signature(siglen, 0x00);
                siglen = signer.SignMessage( prng, (const byte*)&sign[0], sign.size(), (byte*)&signature[0]);
                signature.resize(siglen);
                std::string signature1;
                StringSource ss1((const byte*)&signature[0], signature.size(), true, new HexEncoder(new StringSink(signature1)));
                return signature1;
        }
        bool pVerify(const std::string&nmessage, const std::string& dmessage, const std::string& pubid)
        {
                AutoSeededRandomPool prng;
                HexDecoder decoder2;
                decoder2.Put((byte*)&pubid[0], pubid.size());
                decoder2.MessageEnd();
                ECP::Point q1;
                size_t len = decoder2.MaxRetrievable();
                q1.identity = false;
                q1.x.Decode(decoder2, len/2);
                q1.y.Decode(decoder2, len/2);
                ECDSA<ECP, SHA256>::PublicKey publicKey;
                publicKey.Initialize( ASN1::secp256r1(), q1 );
                ECDSA<ECP, SHA256>::Verifier verifier(publicKey);
                std::string decmessage;
                StringSource ss2(dmessage, true,new HexDecoder(new StringSink(decmessage)));
                bool result = verifier.VerifyMessage( (const byte*)&nmessage[0], nmessage.size(), (const byte*)&decmessage[0], decmessage.size() );
                return result;
        }
    };
}
