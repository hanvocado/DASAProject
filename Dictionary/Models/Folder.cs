namespace Dictionary.Models;

public class Folder {
    private string Name { get; set; }
    private List<Folder> ChildrenFolder { get; set; } = new();
    private LinkedList Items { get; set; } = new();
    public Folder(string name) {
        Name = name;
    }
    public Folder(string name, LinkedList items) {
        Name = name;
        Items = items;
    }

    public void AddChildFolder(string childName) {
        ChildrenFolder.Add(new Folder(childName));
    }
    public List<Folder> GetChildrenFolders(string childName) {
        return ChildrenFolder;
    }
    public string FolderName() {
        return Name;
    }
    public LinkedList GetItems() {
        return Items;
    }
    public void AddItem(string key) {
        Items.AddLast(key);
    }
    public void RemoveItem(string key) {
        Node? nodeToRemove = Items.Find(key);
        Items.Remove(nodeToRemove);
    }

}