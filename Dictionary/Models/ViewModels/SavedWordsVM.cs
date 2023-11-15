namespace Dictionary.Models.ViewModels;

public class SavedWordsVM {
    public Category? Category { get; set; }
    public IEnumerable<string>? Categories { get; set; }
}