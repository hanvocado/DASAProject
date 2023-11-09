using Dictionary.Constants;

namespace Dictionary.Models;

public class HTNode {
    public Word Word { get; set; }
    public HTNode? Next { get; set; }
  
    public HTNode(Word word) {
        Word = word;
        Next = null;
    }
}

public class HashTable {
    public HTNode[] dictArray;

    public HashTable() {
        dictArray = new HTNode[Constant.D_HASHTABLE_SIZE];
    }

    private int HashFunc(string s)
    {
        int g = 2;
        int sum = 0;
        for (int i = 0; i < s.Length; ++i)
            sum += (int)((int)s[i] * Math.Pow(g, i));
        
        return sum % Constant.D_HASHTABLE_SIZE;
    }
    public void Insert(Word word) {
        int hashIndex = HashFunc(word.KeyWord!);
        HTNode node = new HTNode(word);

        // If there's already a word at the hash index, traverse linked list of nodes
        if (dictArray[hashIndex]?.Word != null)
        {
            HTNode travNode = dictArray[hashIndex];

            // Traverse linked list at hash index till the end
            while (travNode.Next != null)
            {
                travNode = travNode.Next;
            }

            travNode.Next = node;
        } else
            dictArray[hashIndex] = node;
    }

    public Word? GetWord(string key) {
        int hashIndex = HashFunc(key);
        if (dictArray[hashIndex]?.Word.KeyWord == key) {
            return dictArray[hashIndex].Word;
        }
        else {
            HTNode travNode = dictArray[hashIndex];
            while (travNode?.Next != null)
            {
                travNode = travNode.Next;
                if (String.Equals(travNode.Word.KeyWord, key, StringComparison.OrdinalIgnoreCase))
                    return travNode.Word;
            }
        }
        return null;
    }
    private int compareString(string str1, string str2)
    {
        return String.Compare(str1, str2, comparisonType: StringComparison.OrdinalIgnoreCase);
    }
}