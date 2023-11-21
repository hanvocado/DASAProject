using System;
using Dictionary.Models;

namespace Dictionary.Services;

public interface IDictionaryService
{
    public Word? LookUp(string? key);
    public List<string> Suggest(string query);
    public string AutoCorrect(string userInput);
}

public class DictionaryService : IDictionaryService
{
    private IWebHostEnvironment _envi;
    private HashTable Dictionary = new();
    private Trie Trie = new();
    private Trie TrieWordsOnly = new();
    public DictionaryService(IWebHostEnvironment envi) {
        _envi = envi;
        CreateDictionary();
        InitTrieWordsOnly();
    }
    public Word? LookUp(string? searchStr)
    {
        if (String.IsNullOrEmpty(searchStr))
            return null;
        return Dictionary.GetWord(searchStr);
    }

    private void CreateDictionary() {
        string filePath = Path.Combine(_envi.WebRootPath, "EnglishDictionary.txt");
        using (var reader = new StreamReader(filePath))
        {
            string? line;
            while ((line = reader.ReadLine()) != null)
            {
                string[] parts = line.Split(' ');

                if (parts.Length >= 3)
                {
                    string keyword = parts[0].ToLower();
                    string partOfSpeech = parts[1];
                    string meaning = string.Join(" ", parts.Skip(2));

                    Trie.Insert(keyword);

                    Word word = new Word(keyword, partOfSpeech, meaning);

                    Dictionary.Insert(word);
                }
            }
        }
    }

    private void InitTrieWordsOnly() {
        string filePath = Path.Combine(_envi.WebRootPath, "words_alpha.txt");
        using (var reader = new StreamReader(filePath)) {
            string? line;
            while ((line = reader.ReadLine()) != null)
            {
                TrieWordsOnly.Insert(line.Trim());
            }
        }
    }

    public List<string> Suggest(string prefix) {
        if(String.IsNullOrEmpty(prefix))
            return new List<string>();
        return Trie.Suggest(prefix.ToLower());
    }

    public string AutoCorrect(string userInput) {
        userInput = userInput.ToLower();
        string prefix = Trie.Find(userInput);
        if (String.Equals(userInput, prefix))
            return "";
        
        List<String> suggestions = TrieWordsOnly.Suggest(prefix);
        
        var distances = suggestions.ToDictionary(s => s, s => CalLevenshteinDis(s, userInput));
        string bestMatch = distances.OrderBy(m => m.Value).First().Key;
        return bestMatch;
    }

    private int CalLevenshteinDis(string str1, string str2) {
        int len1 = str1.Length;
        int len2 = str2.Length;

        int[] prevRow = new int[len2 + 1];
        int[] currRow = new int[len2 + 1];

        for (int j = 0; j <= len2; j++)
        {
            prevRow[j] = j;
        }

        for (int i = 1; i <= len1; i++)
        {
            currRow[0] = i;

            for (int j = 1; j <= len2; j++)
            {
                if (str1[i - 1] == str2[j - 1])
                    currRow[j] = prevRow[j - 1];
                else
                    currRow[j] = 1 + Math.Min(currRow[j - 1], Math.Min(prevRow[j], prevRow[j - 1]));
            }

            Array.Copy(currRow, prevRow, len2 + 1);
        }

        return currRow[len2];
    }
}