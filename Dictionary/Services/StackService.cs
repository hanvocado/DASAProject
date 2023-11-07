using Dictionary.Models;

namespace Dictionary.Services;

public interface IStackService {
    public void Push(string key);
    public Stack GetHistory();
}

public class StackService : IStackService
{
    private Stack History = new ();
    public void Push(string key)
    {
        Node p = new(key);            
        p.Next = History.Top;
        History.Top = p;
    }

    public Stack GetHistory() {
        return History;
    }
}