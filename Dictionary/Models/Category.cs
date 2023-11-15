namespace Dictionary.Models;

public class Category {
    public string Name { get; set; }
    public LinkedList Words { get; set; } = new();
    public string? UserId { get; set; }

    public Category(string name) {
        Name = name;
    }

    public void AddWord(Word word) {
        Words.AddLast(word);
    }
    public void RemoveWord(string key) {
        Words.Remove(key);
    }
}