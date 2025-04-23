using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Games4u_Server.Core
{
    public class Cryption
    {
        public const int N = 256;
        public static int[] sbox;

        public static void XOR(byte seed, byte[] data)
        {
            for (int i = 0; i < data.Length; i++)
            {
                data[i] ^= seed;
            }
        }
        public static void XOR(byte key, string data)
        {
            byte[] output = new byte[data.Length];
            for (int i = 0; i < output.Length; i++)
            {
                output[i] ^= key;
            }
           
            data = Encoding.UTF8.GetString(output);
        }
        public static byte[] RC4Crypt(string key, string array)
        {
            RC4Initialize(key);

            int i = 0, j = 0, k = 0;
            byte[] cipher = new byte[array.Length];
            for (int a = 0; a < array.Length; a++)
            {
                i = (i + 1) % N;
                j = (j + sbox[i]) % N;
                int tempSwap = sbox[i];
                sbox[i] = sbox[j];
                sbox[j] = tempSwap;

                k = sbox[(sbox[i] + sbox[j]) % N];
                int cipherBy = ((int)array[a]) ^ k;
                cipher[a] = (byte)cipherBy;
            }
            return cipher;
        }
        public static void RC4Crypt(string key, byte[] array)
        {
            RC4Initialize(key);

            int i = 0, j = 0, k = 0;
            for (int a = 0; a < array.Length; a++)
            {
                i = (i + 1) % N;
                j = (j + sbox[i]) % N;
                int tempSwap = sbox[i];
                sbox[i] = sbox[j];
                sbox[j] = tempSwap;

                k = sbox[(sbox[i] + sbox[j]) % N];
                int cipherBy = ((int)array[a]) ^ k;
                array[a] = (byte)cipherBy;
            }
        }
        public static void RC4Initialize(string password)
        {
            sbox = new int[N];
            int[] key = new int[N];
            int n = password.Length;
            for (int a = 0; a < N; a++)
            {
                key[a] = (int)password[a % n];
                sbox[a] = a;
            }

            int b = 0;
            for (int a = 0; a < N; a++)
            {
                b = (b + sbox[a] + key[a]) % N;
                int tempSwap = sbox[a];
                sbox[a] = sbox[b];
                sbox[b] = tempSwap;
            }
        }
    }
}
