namespace Dictionary.Models.ViewModels;

public class IndexVM {
    public Word? Word { get; set; }
    public bool WordIsSaved { get; set; } = false;
    public List<string> SavedWords { get; set; }
    public bool BackwardDisabled { get; set; } = false;
    public bool ForwardDisabled { get; set; } = false;

}