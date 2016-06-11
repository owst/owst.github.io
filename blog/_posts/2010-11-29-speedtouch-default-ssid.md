---
title: C#/LINQ to find Thompson/Speedtouch router's default SSID/Access keys
---
Whilst researching content for a wireless security presentation for university,
I came across [this page (now dead, unfortunately)][orig_link]. It details the
algorithm used to calculate the default password for some O2/BE/BT routers.

I thought it would provide an interesting exercise to convert the algorithm to
C#, using LINQ. Here it is:

```csharp
using System;
using System.Linq;
using System.Security.Cryptography;
using System.Text;

class Program
{
    static void Main(string[] args)
    {
        if (args.Length < 4)
        {
            Console.WriteLine("Need 4 args: SSID, year, monthFrom, monthTo");
            Console.WriteLine("SSID = Hex part of SSID");
            Console.WriteLine("Year = 2 digit year e.g. 08");
            Console.WriteLine("monthFrom and monthTo = 0-indexed month numbers");
            Environment.Exit(1);
        }

        string ssid = args[0].ToUpper();
        string year = args[1];
        int monthFrom = int.Parse(args[2]);
        int monthTo = int.Parse(args[3]);

        string key = FindKey(ssid, year, monthFrom, monthTo) ?? "No key found!";

        Console.WriteLine("Key is: " + key);
    }

    static string FindKey(string ssid, string year, int mFrom, int mTo)
    {
        int SSIDlen = ssid.Length;
        var sha1 = SHA1.Create();

        var digitChars = Enumerable.Range((int)'0', 10);
        var letterChars = Enumerable.Range((int)'A', 26);

        // Create hex strings of 0-9 and A-Z.
        var chars = digitChars.Concat(letterChars).Select(i => i.ToString("X"));

        var charTuples = from c1 in chars
                         from c2 in chars
                         from c3 in chars
                         from month in Enumerable.Range(mFrom, mTo - mFrom)
                         select Tuple.Create(month, c1, c2, c3);

        return charTuples.Select(t =>
        {
            // Make a correctly-formed serial.
            var serial = "CP" + year + (t.Item1 < 10 ? "0" : "") + 
                t.Item1 + t.Item2 + t.Item3 + t.Item4;

            var serialBytes = Encoding.UTF8.GetBytes(serial);
            var sha1Bytes = sha1.ComputeHash(serialBytes);

            var hash = String.Join("", sha1Bytes.Select(b => b.ToString("X")));

            // Last 3 bytes are the SSID (
            var testSSID = hash.Substring(hash.Length - SSIDlen);

            // If we've got a match, the first 5 bytes are the default access key.
            return testSSID == ssid ? hash.Substring(0, 10) : "";
        }).FirstOrDefault(s => s != "");
    }
}
```

[orig_link]: http://flash.i.ph/blogs/flash/2009/09/12/default-key-algorithm-in-thomson-most-popular-dsl-router-in-the-pt-meo/
