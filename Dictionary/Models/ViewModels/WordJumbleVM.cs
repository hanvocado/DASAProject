namespace Dictionary.Models.ViewModels;

public class WordJumbleVM {
    public int Index { get; set; }
    public JumbleWord Word { get; set; }

    public WordJumbleVM(int i, JumbleWord word) {
        Index = i;
        Word = word;
    }
}