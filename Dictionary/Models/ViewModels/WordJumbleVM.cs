namespace Dictionary.Models.ViewModels;

public class WordJumbleVM {
    public int Index { get; set; }
    public JumbleWord Word { get; set; }
    public int NoCorrectAnswers { get; set; }
    public int NoQuestions { get; set; }

    public WordJumbleVM(int i, JumbleWord word, int noCorrectAnswers, int noQuestions) {
        Index = i;
        Word = word;
        NoCorrectAnswers = noCorrectAnswers;
        NoQuestions = noQuestions;
    }
}