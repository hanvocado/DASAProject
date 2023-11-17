namespace Dictionary.Models;

public class Word {
    public string? KeyWord { get; set; }
    public string? PartOfSpeech { get; set; }
    public string? Meaning { get; set; }
    public List<string> Categories { get; set; } = new List<string>();
    public Word(string KeyWord, string PartOfSpeech, string Meaning)
    {
        this.KeyWord = KeyWord;
        this.PartOfSpeech = PartOfSpeech;
        this.Meaning = Meaning;
    }
}

public class JumbleWord
{
    public string? KeyWord { get; set; }
    public string? Def { get; set; }
    public string? ShuffledWord { get; set; }
    public JumbleWord(string key, string def, string shuffledWord)
    {
        KeyWord = key;
        Def = def;
        ShuffledWord = shuffledWord;
    }
    public JumbleWord() {}
}