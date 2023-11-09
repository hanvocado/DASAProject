namespace Dictionary.Services;

public interface IUserService {
    public List<string> GetSavedWords();
    public void SaveWord(string key);
    public void DeleteWord(string key);
    public bool IsSaved(string key);

}
public class UserService : IUserService {
    private List<string> SavedWords = new List<string>();
    
    public List<string> GetSavedWords() {
        return SavedWords;
    }

    public void SaveWord(string key) {
        if (!IsSaved(key))
            SavedWords.Add(key);
    }

    public void DeleteWord(string key) {
        SavedWords.Remove(key);
    }

    public bool IsSaved(string key) {
        return SavedWords.Exists(w => w == key);
    }
}