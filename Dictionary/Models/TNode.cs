namespace Dictionary.Models;

public class TNode
{
    public Word Word { get; set; }
    public int Height { get; set; }
    public TNode? Left { get; set; }
    public TNode? Right { get; set; }

    public TNode(Word w)
    {
        Word = w;
        Height = 1;
    }
}