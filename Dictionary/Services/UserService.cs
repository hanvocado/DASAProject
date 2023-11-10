using Dictionary.Models;

namespace Dictionary.Services;

public interface IUserService {
    public void SaveWord(string key, string category);
    public void RemoveWord(string key, string category);
    public LinkedList? GetWords(string category);
    public bool IsSaved(Word? word);

}
public class UserService : IUserService {
    private List<Category> Categories = new();
    private IWebHostEnvironment _env;

    public UserService(IWebHostEnvironment env) {
        _env = env;
        //ReadSavedWords();
    }
    // private void ReadSavedWords() {
    //     string filePath = Path.Combine(_env.WebRootPath, "SavedWords.txt");
    //     using (var reader = new StreamReader(filePath))
    //     {
    //         string? line;
    //         while ((line = reader.ReadLine()) != null) {
    //             SavedWords.AddLast(line);
    //         }
    //     }
    // }
    
    public void SaveWord(string key, string category)
    {
        Category? tag = Categories.Find(t => t.Name == category);
        if (tag == null)
        {
            tag = new Category(category);
            Categories.Add(tag);
        }
        tag.AddWord(key);
    }
    public void RemoveWord(string key, string category)
    {
        Categories.Find(t => t.Name == category)?.RemoveWord(key);
    }

    public bool IsSaved(Word? word) {
        if (word == null) return false;
        return word.Categories == null ? false : word.Categories.Count() > 0;
    }

    public LinkedList? GetWords(string category) {
        return Categories.Find(c => c.Name == category)?.Words;
    }
}