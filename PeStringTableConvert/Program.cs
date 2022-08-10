using System;
using System.IO;
using System.Text;

namespace PeStringTableConvert
{
    /// <summary>
    /// PE文件StringTable数据解析程序。用于辅助创建rc文件中的字符串。
    /// </summary>
    class Program
    {
        static void Main(string[] args)
        {
            // .exe.StringTable 文件来自 PEExplorerV2 导出的二进制数据。
            // string table resource: https://docs.microsoft.com/en-us/windows/win32/menurc/stringtable-resource
            // TODO: 直接转换为rc文件格式的StringTable项。
            string file_path = @"C:\app\diablo2-v1.13c\Diablo II.exe.StringTable";
            FileStream file_stream = new FileStream(file_path, FileMode.Open);
            BinaryReader reader = new BinaryReader(file_stream, Encoding.Unicode);
            int id = 0;
            Console.WriteLine("id, length, string");
            while (file_stream.Position < file_stream.Length)
            {
                ushort length = reader.ReadUInt16();
                byte[] buffer = reader.ReadBytes(length * 2);
                if (buffer.Length != length * 2)
                {
                    throw new Exception("buffer length not match header length");
                }
                string s = Encoding.Unicode.GetString(buffer);
                Console.WriteLine($"{id}, {length}, {GetEscapedString(s)}");
                id++;
            }
        }

        static string GetEscapedString(string s)
        {
            StringBuilder result = new StringBuilder();
            result.Append("\"");
            {
                foreach (char c in s)
                {
                    if (c < 0x20)
                    {
                        switch (c)
                        {
                            case '\r':
                                result.Append("\\r");
                                break;
                            case '\n':
                                result.Append("\\n");
                                break;
                            case '\t':
                                result.Append("\\t");
                                break;
                            case '\0':
                                result.Append("\\0");
                                break;
                            default:
                                string hex = ((int)c).ToString("x2");
                                result.Append("\\x");
                                result.Append(hex);
                                break;
                        }
                    }
                    else
                    {
                        result.Append(c);
                    }
                }
            }
            result.Append("\"");
            return result.ToString();
        }
    }
}
