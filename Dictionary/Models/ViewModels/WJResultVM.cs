namespace Dictionary.Models.ViewModels;

public class WJResultVM {
    public bool Won { get; set; }
    public string OriginalWord { get; set; }
    public int NextIndex { get; set; }

    public WJResultVM(bool won, string originalWord, int nextIndex) {
        Won = won;
        OriginalWord = originalWord;
        NextIndex = nextIndex;
    }
}