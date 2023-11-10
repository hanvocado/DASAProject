using Dictionary.Models;

namespace Dictionary.Services;

public interface IUserService {
    public LinkedList GetSavedWords();
    public List<Folder> GetFolders();
    public void SaveOrUnsaveWord(string key, string folderName);
    public bool IsSaved(string key);

}
public class UserService : IUserService {
    private LinkedList SavedWords = new LinkedList();
    private List<Folder> Folders = new();
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
    public List<Folder> GetFolders() {
        return Folders;
    }
    public void AddFolder(string name) {
        Folders.Add(new Folder(name));
    }
    public void DeleteFolder(string name) {
        Folder? folder = FindFolder(name);
        if (folder == null)
            return;
        Folders.Remove(folder);
    }
    public Folder? FindFolder(string name) {
        return Folders.Find(f => f.FolderName() == name);
    }
    public void SaveOrUnsaveWord(string key, string FolderName) {
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