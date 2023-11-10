using Dictionary.Models;

namespace Dictionary.Services;

public interface IUserService {
    public LinkedList GetSavedWords();
    public List<string> GetFolders();
    public void SaveOrUnsaveWord(string key);
    public bool IsSaved(string key);

}
public class UserService : IUserService {
    private LinkedList SavedWords = new LinkedList();
    private List<string> Folders = new();
    private IWebHostEnvironment _env;

    public UserService(IWebHostEnvironment env) {
        _env = env;
        ReadSavedWords();
    }
    private void ReadSavedWords() {
        string filePath = Path.Combine(_env.WebRootPath, "SavedWords.txt");
        using (var reader = new StreamReader(filePath))
        {
            string? line;
            while ((line = reader.ReadLine()) != null) {
                SavedWords.AddLast(line);
            }
        }
    }
    public LinkedList GetSavedWords() {
        return SavedWords;
    }
    public List<string> GetFolders() {
        return Folders;
    }
    public void AddFolder(string name) {
        Folders.Add(name);
    }
    public void DeleteFolder(string name) {
        Folders.Remove(name);
    }
    public void SaveOrUnsaveWord(string key) {
        Node? p = SavedWords.Find(key);
        if (p == null) {
            SavedWords.AddLast(key);
            
        }
        else {
            SavedWords.Remove(p);
        }
    }

    public bool IsSaved(string key) {
        return SavedWords.Find(key) != null;
    }

    
}