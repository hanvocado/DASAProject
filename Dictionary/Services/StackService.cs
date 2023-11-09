using Dictionary.Models;

namespace Dictionary.Services;

public interface IStackService {
    public void Searched(string? key);
    public string? Forwarded(string? currentKey);
    public string? Backwarded(string? currentKey);
    public bool ForwardDisabled();
    public bool BackwardDisabled();
}

public class StackService : IStackService
{
    private Stack Forward = new Stack();
    private Stack Backward = new Stack();

    public void Searched(string? key) {
        if (!String.Equals(Backward.Top(), key, StringComparison.OrdinalIgnoreCase)) {
            Backward.Push(key);
        }
    }

    public string? Forwarded(string? currentKey) {
        if (!Forward.IsEmpty()) {
            Backward.Push(currentKey);
        }
        string? keyToForwardTo = Forward.Top();
        Forward.Pop();

        return keyToForwardTo;
    }
    public string? Backwarded(string? currentKey) {
        if (!Backward.IsEmpty()) {
            Forward.Push(currentKey);
        }
        Backward.Pop();
        string? keyToBackwardTo = Backward.Top();
        Backward.Pop();

        return keyToBackwardTo;
    }

    public bool ForwardDisabled()
    {
        return Forward.IsEmpty();
    }

    public bool BackwardDisabled()
    {
        return Backward.IsEmpty();
    }
}