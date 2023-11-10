namespace Dictionary.Models;

public class Word {
    public string? KeyWord { get; set; }
    public string? PartOfSpeech { get; set; }
    public string? Meaning { get; set; }
    public List<Category>? Categories { get; set; }
    public Word(string KeyWord, string PartOfSpeech, string Meaning)
    {
        this.KeyWord = KeyWord;
        this.PartOfSpeech = PartOfSpeech;
        this.Meaning = Meaning;
    }
}