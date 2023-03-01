#include <iostream>
#include <bitset>
#include <gmp.h>
#include <gmpxx.h>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
using namespace std;
using std::cout;
using std::string;
#define STR "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
#define SHA2_SHFR(x, n) (x >> n)
#define SHA2_ROTR(x, n) ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define SHA2_ROTL(x, n) ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define SHA2_CH(x, y, z) ((x & y) ^ (~x & z))
#define SHA2_MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA256_F1(x) (SHA2_ROTR(x, 2) ^ SHA2_ROTR(x, 13) ^ SHA2_ROTR(x, 22))
#define SHA256_F2(x) (SHA2_ROTR(x, 6) ^ SHA2_ROTR(x, 11) ^ SHA2_ROTR(x, 25))
#define SHA256_F3(x) (SHA2_ROTR(x, 7) ^ SHA2_ROTR(x, 18) ^ SHA2_SHFR(x, 3))
#define SHA256_F4(x) (SHA2_ROTR(x, 17) ^ SHA2_ROTR(x, 19) ^ SHA2_SHFR(x, 10))
#define SHA2_UNPACK32(x, str)              \
    {                                      \
        *((str) + 3) = (uint8)((x));       \
        *((str) + 2) = (uint8)((x) >> 8);  \
        *((str) + 1) = (uint8)((x) >> 16); \
        *((str) + 0) = (uint8)((x) >> 24); \
    }
#define SHA2_PACK32(str, x)                                                                                                                \
    {                                                                                                                                      \
        *(x) = ((uint32) * ((str) + 3)) | ((uint32) * ((str) + 2) << 8) | ((uint32) * ((str) + 1) << 16) | ((uint32) * ((str) + 0) << 24); \
    }

class SHA256
{
protected:
    typedef unsigned char uint8;
    typedef unsigned int uint32;
    typedef unsigned long long uint64;

    const static uint32 sha256_k[];
    static const unsigned int SHA_256_BLOCK_SIZE = (512 / 8);

public:
    void init();
    void update(const unsigned char *message, unsigned int len);
    void final(unsigned char *digest);
    static const unsigned int DIGEST_SIZE = (256 / 8);

protected:
    void transform(const unsigned char *message, unsigned int block_nb);
    unsigned int m_tot_len;
    unsigned int m_len;
    unsigned char m_block[2 * SHA_256_BLOCK_SIZE];
    uint32 m_h[8];
};

// P, Q -> prepared
mpz_class P("70370393959675521820500782129455829046834624191193040774778174186390861020432999560509479154049871458971941351584959609981703698259784491166467017313830811268279206174600595835094806956703023301214538079175799918922889270950498970147750593045730876854838583096128502835448395964315594558492757738078102448063");
mpz_class Q("138546997122076233229845236559079110806642277731618790000419567680695298567304830060061354017372881645373283309640996354308746527346324698910039969835699225730201932391098735680425536942180270112225329355005519944092453869875019638296668830952322651286967291745849935332582547395526139086212637793518775176423");
// plaintext -> prepared
string pt_str = "wujianlin";
string pt_ascii = "";
string pt_bit = "";
string pt_oaep = "";
// ciphertext
string ct_ascii = "";
string ct_oaep = "";
// decrypttext
string dt_ascii = "";
string dt_oaep = "";
// public key -> prepared
mpz_class publicKey("20337269");
// private key -> prepared by initial()
mpz_class privateKey;
// n = pq -> prepared
mpz_class n("9749606769410535268135797852760620092846648890667427331043591893382030201562344928724232791251914777336103391364353899213785611207557831229718147731326555588087277219195124528669660725047038588229738410943216166046350241881823207407071727928601183011784055936927151627200769241125655092172823653363530678179407653896505197072333524748143535989218969023942104077319256027859095182567701022564859461218923783701762592553231441615339368193711844587918991694755078947186417028337507737764975537540693244782578989069185283641254808984706889931254104559948066304965237579275654780610956855964048245141279467405951019618649");
// phi_n = (p-1)(q-1) -> prepared
mpz_class phi_n("9749606769410535268135797852760620092846648890667427331043591893382030201562344928724232791251914777336103391364353899213785611207557831229718147731326555588087277219195124528669660725047038588229738410943216166046350241881823207407071727928601183011784055936927151627200769241125655092172823653363530678179198736505423445317283178729455001049365492122019292246544058285992009022979963192944288628047501030597417367892005485651048917968105735397842484707605548910187935889771808406249455193641809951369139121635003963778239465843881371322809685135950012776823431704433676342442925912604206511496574071874354141994164");
// random number
string r("asdfghjklqwertyuiolmvftyuiolmnbgfd");

void initial();
void rsaEncrypt();
void rsaDecrypt();
void stringToAscii();
string MGF(string);
void oaepPadding();
string sha256(string);
void oaepUnpadding();
string decToBit(string);

int main()
{
    cout << "hello gmp" << '\n';
    initial();
    stringToAscii();
    oaepPadding();
    rsaEncrypt();
    rsaDecrypt();
    oaepUnpadding();
    cout << "\nstill fixing\n";
}

void initial()
{
    mpz_invert(privateKey.get_mpz_t(), publicKey.get_mpz_t(), phi_n.get_mpz_t());
}

void rsaEncrypt()
{
    string pt_oaep_hex = "";
    for (int i = 0; i < 2048;)
    {
        string temp = pt_oaep.substr(i, 4);
        if (temp == "0000")
        {
            pt_oaep_hex += '0';
        }
        else if (temp == "0001")
        {
            pt_oaep_hex += '1';
        }
        else if (temp == "0010")
        {
            pt_oaep_hex += '2';
        }
        else if (temp == "0011")
        {
            pt_oaep_hex += '3';
        }
        else if (temp == "0100")
        {
            pt_oaep_hex += '4';
        }
        else if (temp == "0101")
        {
            pt_oaep_hex += '5';
        }
        else if (temp == "0110")
        {
            pt_oaep_hex += '6';
        }
        else if (temp == "0111")
        {
            pt_oaep_hex += '7';
        }
        else if (temp == "1000")
        {
            pt_oaep_hex += '8';
        }
        else if (temp == "1001")
        {
            pt_oaep_hex += '9';
        }
        else if (temp == "1010")
        {
            pt_oaep_hex += 'a';
        }
        else if (temp == "1011")
        {
            pt_oaep_hex += 'b';
        }
        else if (temp == "1100")
        {
            pt_oaep_hex += 'c';
        }
        else if (temp == "1101")
        {
            pt_oaep_hex += 'd';
        }
        else if (temp == "1110")
        {
            pt_oaep_hex += 'e';
        }
        else
        {
            pt_oaep_hex += 'f';
        }

        i = i + 4;
    }

    mpz_class pt_mc(pt_oaep_hex, 16);
    mpz_class ct_mc;
    cout << "pt_oaep_hex = " << pt_oaep_hex << '\n';
    cout << "pt_mc = " << pt_mc << '\n';
    mpz_powm(ct_mc.get_mpz_t(), pt_mc.get_mpz_t(), publicKey.get_mpz_t(), n.get_mpz_t());
    ct_oaep = ct_mc.get_str();
    cout << "ct_mc = " << ct_mc << '\n';
}

void rsaDecrypt()
{
    mpz_class dt_mc;
    mpz_class ct_mc(ct_oaep);
    mpz_powm(dt_mc.get_mpz_t(), ct_mc.get_mpz_t(), privateKey.get_mpz_t(), n.get_mpz_t());
    dt_oaep = dt_mc.get_str();
}
// pt_str -> pt_ascii(decimal)
void stringToAscii()
{
    for (int i = 0; i < pt_str.length(); i++)
    {
        bitset<8> temp(pt_str[i]);
        pt_ascii += to_string(temp.to_ulong());
    }
}

const unsigned int SHA256::sha256_k[64] = // UL = uint32
    {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
     0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
     0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
     0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
     0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
     0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
     0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
     0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
     0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
     0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
     0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
     0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
     0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
     0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
     0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
     0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

void SHA256::transform(const unsigned char *message, unsigned int block_nb)
{
    uint32 w[64];
    uint32 wv[8];
    uint32 t1, t2;
    const unsigned char *sub_block;
    int i;
    int j;
    for (i = 0; i < (int)block_nb; i++)
    {
        sub_block = message + (i << 6);
        for (j = 0; j < 16; j++)
        {
            SHA2_PACK32(&sub_block[j << 2], &w[j]);
        }
        for (j = 16; j < 64; j++)
        {
            w[j] = SHA256_F4(w[j - 2]) + w[j - 7] + SHA256_F3(w[j - 15]) + w[j - 16];
        }
        for (j = 0; j < 8; j++)
        {
            wv[j] = m_h[j];
        }
        for (j = 0; j < 64; j++)
        {
            t1 = wv[7] + SHA256_F2(wv[4]) + SHA2_CH(wv[4], wv[5], wv[6]) + sha256_k[j] + w[j];
            t2 = SHA256_F1(wv[0]) + SHA2_MAJ(wv[0], wv[1], wv[2]);
            wv[7] = wv[6];
            wv[6] = wv[5];
            wv[5] = wv[4];
            wv[4] = wv[3] + t1;
            wv[3] = wv[2];
            wv[2] = wv[1];
            wv[1] = wv[0];
            wv[0] = t1 + t2;
        }
        for (j = 0; j < 8; j++)
        {
            m_h[j] += wv[j];
        }
    }
}

void SHA256::init()
{
    m_h[0] = 0x6a09e667;
    m_h[1] = 0xbb67ae85;
    m_h[2] = 0x3c6ef372;
    m_h[3] = 0xa54ff53a;
    m_h[4] = 0x510e527f;
    m_h[5] = 0x9b05688c;
    m_h[6] = 0x1f83d9ab;
    m_h[7] = 0x5be0cd19;
    m_len = 0;
    m_tot_len = 0;
}

void SHA256::update(const unsigned char *message, unsigned int len)
{
    unsigned int block_nb;
    unsigned int new_len, rem_len, tmp_len;
    const unsigned char *shifted_message;
    tmp_len = SHA_256_BLOCK_SIZE - m_len;
    rem_len = len < tmp_len ? len : tmp_len;
    memcpy(&m_block[m_len], message, rem_len);
    if (m_len + len < SHA_256_BLOCK_SIZE)
    {
        m_len += len;
        return;
    }
    new_len = len - rem_len;
    block_nb = new_len / SHA_256_BLOCK_SIZE;
    shifted_message = message + rem_len;
    transform(m_block, 1);
    transform(shifted_message, block_nb);
    rem_len = new_len % SHA_256_BLOCK_SIZE;
    memcpy(m_block, &shifted_message[block_nb << 6], rem_len);
    m_len = rem_len;
    m_tot_len += (block_nb + 1) << 6;
}

void SHA256::final(unsigned char *digest)
{
    unsigned int block_nb;
    unsigned int pm_len;
    unsigned int len_b;
    int i;
    block_nb = (1 + ((SHA_256_BLOCK_SIZE - 9) < (m_len % SHA_256_BLOCK_SIZE)));
    len_b = (m_tot_len + m_len) << 3;
    pm_len = block_nb << 6;
    memset(m_block + m_len, 0, pm_len - m_len);
    m_block[m_len] = 0x80;
    SHA2_UNPACK32(len_b, m_block + pm_len - 4);
    transform(m_block, block_nb);
    for (i = 0; i < 8; i++)
    {
        SHA2_UNPACK32(m_h[i], &digest[i << 2]);
    }
}

std::string sha256(std::string input)
{
    unsigned char digest[SHA256::DIGEST_SIZE];
    memset(digest, 0, SHA256::DIGEST_SIZE);

    SHA256 ctx = SHA256();
    ctx.init();
    ctx.update((unsigned char *)input.c_str(), input.length());
    ctx.final(digest);

    char buf[2 * SHA256::DIGEST_SIZE + 1];
    buf[2 * SHA256::DIGEST_SIZE] = 0;
    for (int i = 0; i < SHA256::DIGEST_SIZE; i++)
        sprintf(buf + i * 2, "%02x", digest[i]);
    return std::string(buf);
}

// pt OAEP padding
void oaepPadding()
{
    // 1. expand pt and r to 1024 bit in bit
    for (int i = 0; i < pt_str.length(); i++)
    {
        bitset<8> temp(pt_str[i]);
        pt_bit += temp.to_string();
    }
    pt_bit = STR + pt_bit;
    pt_bit = pt_bit.substr(pt_bit.length() - 1024, 1024);

    string r_bit = "";
    for (int i = 0; i < r.length(); i++)
    {
        bitset<8> temp(r[i]);
        r_bit += temp.to_string();
    }
    r_bit = STR + r_bit;
    r_bit = r_bit.substr(r_bit.length() - 1024, 1024);
    cout << "\nnow we type r_hex = ";
    for (int i = 0; i < 1024;)
    {
        string temp = r_bit.substr(i, 4);
        if (temp == "0000")
        {
            cout << '0';
        }
        else if (temp == "0001")
        {
            cout << '1';
        }
        else if (temp == "0010")
        {
            cout << '2';
        }
        else if (temp == "0011")
        {
            cout << '3';
        }
        else if (temp == "0100")
        {
            cout << '4';
        }
        else if (temp == "0101")
        {
            cout << '5';
        }
        else if (temp == "0110")
        {
            cout << '6';
        }
        else if (temp == "0111")
        {
            cout << '7';
        }
        else if (temp == "1000")
        {
            cout << '8';
        }
        else if (temp == "1001")
        {
            cout << '9';
        }
        else if (temp == "1010")
        {
            cout << 'a';
        }
        else if (temp == "1011")
        {
            cout << 'b';
        }
        else if (temp == "1100")
        {
            cout << 'c';
        }
        else if (temp == "1101")
        {
            cout << 'd';
        }
        else if (temp == "1110")
        {
            cout << 'e';
        }
        else
        {
            cout << 'f';
        }

        i = i + 4;
    }
    cout << '\n';
    // 2. type exchange between bitset and string
    string r_bit_mgf = MGF(r_bit);
    string pt_bit_mgf = MGF(pt_bit);
    bitset<1024> r_bit_bs(r_bit);
    bitset<1024> pt_bit_bs(pt_bit);
    bitset<1024> r_bit_mgf_bs(r_bit_mgf);
    bitset<1024> pt_bit_mgf_bs(pt_bit_mgf);
    // 3. generate oaepPadding's output
    bitset<1024> p1 = pt_bit_bs ^ r_bit_mgf_bs;
    bitset<1024> p2 = pt_bit_mgf_bs ^ r_bit_bs;
    pt_oaep = p1.to_string() + p2.to_string();
}

string MGF(string input)
{
    string m = input, ret = "", ret_hex;
    // 2. m||0, m||1, m||2, m||3
    string input_ascii = "";
    for (int i = 0; i < 1024; i = i + 8)
    {
        bitset<8> temp(input.substr(i, 8));
        input_ascii += (temp.to_ulong());
    }
    bitset<8> temp0(0);
    input_ascii += temp0.to_ulong();
    // 3. sha256 hash
    bitset<8> tempi(0);
    string input1 = input_ascii;
    input1 += (tempi.to_ulong());
    cout << "input1 " << input1.c_str() << '\n';
    cout << "temp0 " << tempi.to_ulong() << '\n'; 
    string Hash1 = sha256(input1);

    tempi = 1;
    string input2 = input_ascii;
    input2 += (tempi.to_ulong());
    string Hash2 = sha256(input2);

    tempi = 2;
    string input3 = input_ascii;
    input3 += (tempi.to_ulong());
    string Hash3 = sha256(input3);

    tempi = 3;
    string input4 = input_ascii;
    input4 += (tempi.to_ulong());
    string Hash4 = sha256(input4);
    cout << "hash1 = " << Hash1 << '\n';
    // 4. H1 || H2 || H3 || H4
    string Hash = Hash1 + Hash2 + Hash3 + Hash4;
    // 5. highest 2 byte set 0
    ret_hex = "00" + Hash.substr(2, 256);
    // 6. hex -> bit
    for (int i = 0; i < 256; i++)
    {
        switch (ret_hex[i])
        {
        case '0':
            ret += "0000";
            break;
        case '1':
            ret += "0001";
            break;
        case '2':
            ret += "0010";
            break;
        case '3':
            ret += "0011";
            break;
        case '4':
            ret += "0100";
            break;
        case '5':
            ret += "0101";
            break;
        case '6':
            ret += "0110";
            break;
        case '7':
            ret += "0111";
            break;
        case '8':
            ret += "1000";
            break;
        case '9':
            ret += "1001";
            break;
        case 'a':
            ret += "1010";
            break;
        case 'b':
            ret += "1011";
            break;
        case 'c':
            ret += "1100";
            break;
        case 'd':
            ret += "1101";
            break;
        case 'e':
            ret += "1110";
            break;
        case 'f':
            ret += "1111";
            break;
        }
    }

    return ret;
}

void oaepUnpadding()
{
    string dt_oaep_bit = decToBit(dt_oaep);
    dt_oaep_bit = STR + dt_oaep_bit;
    dt_oaep_bit = STR + dt_oaep_bit;
    dt_oaep_bit = dt_oaep_bit.substr(dt_oaep_bit.length() - 2048, 2048);

    string p1 = dt_oaep_bit.substr(0, 1024);
    string p2 = dt_oaep_bit.substr(1024, 1024);
    string p1_mgf = MGF(p1);

    bitset<1024> p1_bs(p1);
    bitset<1024> p2_bs(p2);
    bitset<1024> p1_mgf_bs(p1_mgf);
    bitset<1024> tempstr(p1_mgf_bs ^ p2_bs);

    bitset<1024> temp(MGF(tempstr.to_string()));
    bitset<1024> dt_bit_bs(p1_bs ^ temp);
    if (dt_oaep_bit == pt_oaep)
    {
        cout << "\ndt and pt after padding is the same\n";
    }
    
    cout << "dt_bit_bs = " << dt_bit_bs << '\n';
}

string decToBit(string input)
{
    mpz_class num(input);
    string ret;
    while (num != 0)
    {
        ret = (num % 2 == 0 ? "0" : "1") + ret;
        num /= 2;
    }
    return ret;
}
