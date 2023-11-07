namespace Dictionary.Models;

public class User {
    public Stack SearchHistory { get; set; }

    public User() {
        SearchHistory = new Stack();
    }
}