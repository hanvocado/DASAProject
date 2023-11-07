using Dictionary.Models;

namespace Dictionary.Services;

public interface IStackService {
    public void Push(Stack stack, string key);
}

public class StackService : IStackService
{
    public void Push(Stack stack, string key)
    {
        Node p = new(key);            
        p.Next = stack.Top;
        stack.Top = p;
    }
}