using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text.RegularExpressions;
using System.Security.Cryptography;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;

namespace Games4u_Server.Core
{
    public class Array
    {
        private static Random random = new Random( );

        public static Bitmap ConvertToBitmap( byte[ ] byteArray )
        {
            using ( MemoryStream ms = new MemoryStream( byteArray ) )
            {
                Bitmap img = ( Bitmap )Image.FromStream( ms );
                return img;
            }

        }
        public static byte[ ] RemoveBytes( byte[ ] byteArray, int start )
        {
            byte[] tmp = new byte[ byteArray.Length ];
            Buffer.BlockCopy( byteArray, start, tmp, 0, byteArray.Length - start );

            return tmp;
        }
        public static string Base64Encode(string plainText)
        {
            var plainTextBytes = System.Text.Encoding.UTF8.GetBytes(plainText);
            return System.Convert.ToBase64String(plainTextBytes);
        }
        public static void SaveJPG100( Bitmap bmp, string filename )
        {
            EncoderParameters encoderParameters = new EncoderParameters( 1 );
            encoderParameters.Param[ 0 ] = new EncoderParameter( System.Drawing.Imaging.Encoder.Quality, 100L );
            bmp.Save( filename, GetEncoder( ImageFormat.Jpeg ), encoderParameters );
        }

        public static ImageCodecInfo GetEncoder( ImageFormat format )
        {
            ImageCodecInfo[] codecs = ImageCodecInfo.GetImageDecoders( );

            foreach ( ImageCodecInfo codec in codecs )
            {
                if ( codec.FormatID == format.Guid )
                {
                    return codec;
                }
            }

            return null;
        }
        public static string GetTextBetween(string value, string a, string b)
        {
            int posA = value.IndexOf(a);
            int posB = value.LastIndexOf(b);
            if (posA == -1)
            {
                return "";
            }
            if (posB == -1)
            {
                return "";
            }
            int adjustedPosA = posA + a.Length;
            if (adjustedPosA >= posB)
            {
                return "";
            }
            return value.Substring(adjustedPosA, posB - adjustedPosA);
        }
        public static string RemoveWhitespace(string input)
        {
            int j = 0, inputlen = input.Length;
            char[] newarr = new char[inputlen];

            for (int i = 0; i < inputlen; ++i)
            {
                char tmp = input[i];

                if (!char.IsWhiteSpace(tmp))
                {
                    newarr[j] = tmp;
                    ++j;
                }
            }

            return new String(newarr, 0, j);
        }
        public static void ShuffleList<E>( IList<E> list )
        {
            if ( list.Count > 1 )
            {
                for ( int i = list.Count - 1; i >= 0; i-- )
                {
                    E tmp = list[ i ];
                    int randomIndex = random.Next( i + 1 );

                    //Swap elements
                    list[ i ] = list[ randomIndex ];
                    list[ randomIndex ] = tmp;
                }
            }
        }
        public static string GetString(byte[] bytes)
        {
            char[] chars = new char[bytes.Length / sizeof(char)];
            System.Buffer.BlockCopy(bytes, 0, chars, 0, bytes.Length);
            return new string(chars);
        }
        public static string GetUniqueKey(int maxSize)
        {
            char[] chars = new char[62];
            chars =
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890".ToCharArray();
            byte[] data = new byte[1];
            using (RNGCryptoServiceProvider crypto = new RNGCryptoServiceProvider())
            {
                crypto.GetNonZeroBytes(data);
                data = new byte[maxSize];
                crypto.GetNonZeroBytes(data);
            }
            StringBuilder result = new StringBuilder(maxSize);
            foreach (byte b in data)
            {
                result.Append(chars[b % (chars.Length)]);
            }
            return result.ToString();
        }
        public static string RemoveWord(string stringToClean, string wordsToRemove)
        {
            string[] splitWords = wordsToRemove.Split(new Char[] { ' ' });

            string pattern = "";

            foreach (string word in splitWords)
            {
                pattern = @"\b" + word + "\b";
                stringToClean = Regex.Replace(stringToClean, pattern, "");
            }

            return stringToClean;
        }
        public static byte[] GetBytes(string str)
        {
            byte[] bytes = new byte[str.Length * sizeof(char)];
            System.Buffer.BlockCopy(str.ToCharArray(), 0, bytes, 0, bytes.Length);
            return bytes;
        }
        public static string GetTextAfter(string value, string a)
        {
            int posA = value.LastIndexOf(a);
            if (posA == -1)
            {
                return "";
            }
            int adjustedPosA = posA + a.Length;
            if (adjustedPosA >= value.Length)
            {
                return "";
            }
            return value.Substring(adjustedPosA);
        }
        public static int FindBytes( byte[ ] src, byte[ ] find )
        {
            int index = -1;
            int matchIndex = 0;
            for ( int i = 0; i < src.Length; i++ )
            {
                if ( src[ i ] == find[ matchIndex ] )
                {
                    if ( matchIndex == (find.Length - 1) )
                    {
                        index = i - matchIndex;
                        break;
                    }
                    matchIndex++;
                }
                else
                {
                    matchIndex = 0;
                }

            }
            return index;
        }
        public static byte[] CombineByteArrays(byte[] arrayA, byte[] arrayB)
        {
            byte[] outputBytes = new byte[arrayA.Length + arrayB.Length];
            Buffer.BlockCopy(arrayA, 0, outputBytes, 0, arrayA.Length);
            Buffer.BlockCopy(arrayB, 0, outputBytes, arrayA.Length, arrayB.Length);
            return outputBytes;
        }
        public static int GenerateRandomNumber(int Low, int High)
        {
            Random rndNum = new Random(int.Parse(Guid.NewGuid().ToString().Substring(0, 8), System.Globalization.NumberStyles.HexNumber));

            int rnd = rndNum.Next(Low, High);

            return rnd;
        }
    }
}
