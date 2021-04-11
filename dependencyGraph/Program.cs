using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace Il2CppDependencyVisualizer
{
    class Program
    {
        static Dictionary<string, string> output = new Dictionary<string, string>(10000);
        static Dictionary<string, string> files;
        static Regex forwardDeclRegex = new Regex(@"namespace GlobalNamespace \{(.+?)\}", RegexOptions.Singleline | RegexOptions.Compiled);
        static Regex classRegex = new Regex(@"class (\w+);", RegexOptions.Compiled);

        static void Main(string[] args)
        {
            Console.WriteLine("Reading files into memory...");
            files = Directory.EnumerateFiles(@"s:\QuestModding\NjsTweaks\extern\codegen\include\GlobalNamespace\").ToDictionary(x => Path.GetFileNameWithoutExtension(x), x => File.ReadAllText(x));
            var i = 0;
            foreach (var file in files.Keys)
            {
                i++;
                
                AddDependencies(file);
                Console.WriteLine($"{(int)(i / ((double)files.Count)*100)}%");
            }
            var b = new StringBuilder(output.Count+10);
            b.AppendLine("digraph G {");
            output.Values.ToList().ForEach(x => {
                b.Append("  ");
                b.AppendLine(x);
            });
            b.AppendLine("}");
            File.WriteAllText(Path.Combine(Directory.GetCurrentDirectory(), "output.g"), b.ToString());
        }

        static void AddDependencies(string file)
        {
            if (output.ContainsKey(file)) return;
            var forwardDecl = forwardDeclRegex.Match(files[file]).Value;
            var classes = classRegex.Matches(forwardDecl).Select(m => m.Groups[1].Value).ToList();
            classes.ForEach(x => {
                output.TryAdd(file, $"\"{file}\" -> \"{x}\"");
                AddDependencies(x);
            });
        }
    }
}
