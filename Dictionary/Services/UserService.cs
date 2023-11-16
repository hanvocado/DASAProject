using Dictionary.Models;

namespace Dictionary.Services;

public interface IUserService {
    public void SaveWord(Word word, string category);
    public void RemoveWord(Word word, string category);
    public Category? Category(string category);
    public void CreateCategory(string name);
    public string? GetWordCategory(string keyword);
    public List<Category> GetCategories();
    public bool IsSaved(Word? word);

}
public class UserService : IUserService {
    private List<Category> Categories = new();

    public List<Category> GetCategories() {
        return Categories;
    }
    public void SaveWord(Word word, string category)
    {
        if (word.Categories.Contains(category))
            return;
        Category? cat = Categories.Find(t => t.Name == category);
        if (cat == null)
        {
            cat = new Category(category);
            Categories.Add(cat);
        }
        cat.AddWord(word);
        word.Categories.Add(category);
    }
    public void RemoveWord(Word word, string category)
    {
        Categories.Find(t => t.Name == category)?.RemoveWord(word.KeyWord!);
        word.Categories.Remove(category);
    }

    public bool IsSaved(Word? word) {
        if (word == null) return false;
        return word.Categories != null && word.Categories.Count > 0;
    }

    public Category? Category(string category) {
        return Categories.Find(c => c.Name == category);
    }

    public string? GetWordCategory(string keyword) {
        foreach (Category cat in Categories) {
            if (cat.Words.Find(keyword) != null)
                return cat.Name;
        }
        return null;
    }
    public void CreateCategory(string name) {
        Category cat = new(name);
        Categories.Add(cat);
    }
}