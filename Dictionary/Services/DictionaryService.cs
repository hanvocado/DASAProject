using System;
using Dictionary.Models;

namespace Dictionary.Services;

public interface IDictionaryService
{
    public Word? LookUp(string? key);
    public List<string> Suggest(string query);
}

public class DictionaryService : IDictionaryService
{
    private IWebHostEnvironment _envi;
    private HashTable Dictionary = new();
    private Trie Trie = new();
    public DictionaryService(IWebHostEnvironment envi) {
        _envi = envi;
        CreateDictionary();
    }
    public Word? LookUp(string? searchStr)
    {
        if (String.IsNullOrEmpty(searchStr))
            return null;
        return Dictionary.GetWord(searchStr);
    }

    private void CreateDictionary() {
        string filePath = Path.Combine(_envi.WebRootPath, "EnglishDictionary.txt");
        Console.WriteLine(filePath);
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

    public List<string> Suggest(string query) {
        if(String.IsNullOrEmpty(query))
            return new List<string>();
        return Trie.Suggest(query.ToLower());
    }
}