using Dictionary.Models;

namespace Dictionary.Services;

public interface IUserService {
    public LinkedList GetSavedWords();
    public void SaveOrUnsaveWord(string key);
    public bool IsSaved(string key);

}
public class UserService : IUserService {
    private LinkedList SavedWords = new LinkedList();
    private List<string> Forders = new();
    
    public LinkedList GetSavedWords() {
        return SavedWords;
    }

    public void SaveOrUnsaveWord(string key) {
        Node? p = find(SavedWords, key);
        if (p == null)
            addLast(SavedWords, key);
        else {
            if (SavedWords.Head == null) return;
            if (SavedWords.Head == p) {
                if (SavedWords.Tail == p) {
                    SavedWords.Head = SavedWords.Tail = null;
                } else {
                    SavedWords.Head = SavedWords.Head.Next;
                }
            } else {
                Node? q = SavedWords.Head;
                while (q != null && q.Next != p)
                {
                    q = q.Next;
                }
                if (q == null) return;

                q.Next = p.Next;
                if (SavedWords.Tail == p)
                    SavedWords.Tail = q;
            }
            p = null;
        }
    }

    public bool IsSaved(string key) {
        return find(SavedWords, key) != null;
    }

    private void addLast(LinkedList li, string key) {
        Node node = new Node(key);
        if (li.Head == null) {
            li.Head = node;
            li.Tail = node;
        } else {
            li.Tail!.Next = node;
            li.Tail = node;
        }
    }

    private Node? find(LinkedList li, string key) {
        for (Node? p = li.Head; p != null; p = p.Next) {
            if (String.Equals(key, p.Key, StringComparison.OrdinalIgnoreCase)) {
                return p;
            }
        }
        return null;
    }
}