#ifndef RESEED_H
#define RESEED_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cryptopp/hmac.h>
#include "Identity.h"
#include "aes.h"

namespace i2p
{
namespace data
{

	class Reseeder
	{
		typedef Tag<512> PublicKey;	
		
		public:
		
			Reseeder();
			~Reseeder();
			bool reseedNow(); // depreacted
			int ReseedNowSU3 ();

			void LoadCertificates ();

			std::string HttpsRequest (const std::string& address); // TODO: move to private section
			
		private:

			void LoadCertificate (const std::string& filename);
			std::string LoadCertificate (CryptoPP::ByteQueue& queue); // returns issuer's name
			
			int ReseedFromSU3 (const std::string& host);
			int ProcessSU3File (const char * filename);	
			int ProcessSU3Stream (std::istream& s);	

			bool FindZipDataDescriptor (std::istream& s);
			
		private:	

			std::map<std::string, PublicKey> m_SigningKeys;
	};

	class TlsSession
	{
		public:

			TlsSession (const std::string& address);

		private:

			CryptoPP::RSA::PublicKey ExtractPublicKey (const uint8_t * certificate, size_t len);
			void PRF (const uint8_t * secret, const char * label, const uint8_t * random, size_t randomLen,
				size_t len, uint8_t * buf);
			void CalculateMAC (uint8_t type, uint64_t seqn, const uint8_t * buf, size_t len, uint8_t * mac);
			size_t Encrypt (const uint8_t * in, size_t len, const uint8_t * mac, uint8_t * out);
			size_t Decrypt (uint8_t * in, size_t len, uint8_t * out);		

		private:

			CryptoPP::AutoSeededRandomPool m_Rnd;
			i2p::crypto::CBCEncryption m_Encryption;
			i2p::crypto::CBCDecryption m_Decryption; 
			uint8_t m_MacKey[32]; // client	
	};
}
}

#endif
