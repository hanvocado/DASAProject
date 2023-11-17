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
    public JumbleWord? PlayWordJumble(string? cat, int i);

}
public class UserService : IUserService {
    private List<Category> Categories = new();
    private Random random = new Random();
    private List<JumbleWord>? JumbleWords;

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

    private List<JumbleWord> InitWordJumble(string? category)
    {
        Category? cat = Categories.Find(c => c.Name == category);
        cat ??= Categories[random.Next(Categories.Count + 1)];
        List<Word> words = Words(cat);
        ShuffleWords(words);
        JumbleWords = new();
        foreach (Word word in words) {
            JumbleWords.Add(new JumbleWord(word.KeyWord!, word.Meaning!, ShuffleLetter(word.KeyWord!)));
        }
        return JumbleWords;      
    }
    public JumbleWord? PlayWordJumble(string? cat, int i) {
        if (i < 0) return null;
        if (i == 0 || JumbleWords == null || JumbleWords.Count == 0) 
            InitWordJumble(cat);
        if (JumbleWords!.Count == 0) return null;
        if (JumbleWords!.Count <= i)
            return new JumbleWord();
        return JumbleWords[i];
    }
    private string ShuffleLetter(string origin) {
        char[] letters = origin.ToCharArray();
        for (int i = letters.Length -1; i > 0; i--) {
            int j = random.Next(i+1);
            char temp = letters[i];
            letters[i] = letters[j];
            letters[j] = temp;
        }
        return new string(letters);
    }
    private void ShuffleWords(List<Word> origin) {
        for (int i = origin.Count - 1; i > 0; i--) {
            int j = random.Next(i + 1);
            Word temp = origin[i];
            origin[i] = origin[j];
            origin[j] = temp;
        }
    }

    private List<Word> Words(Category cat)
    {
        List<Word> words = new List<Word>();
        for (LNode? n = cat.Words.Head; n != null; n = n.Next)
        {
            words.Add(n.Word);
        }
        return words;
    }
}