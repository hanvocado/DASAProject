using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Dictionary.Models;
using Dictionary.Services;
using Dictionary.Models.ViewModels;
using Microsoft.AspNetCore.Http;

namespace Dictionary.Controllers;

public class HomeController : Controller
{
    public readonly IDictionaryService _dictionary;
    public readonly IBrowseService _stack;
    public readonly IUserService _user;
    public HomeController(IDictionaryService dictionary, IBrowseService stack, IUserService user)
    {
        _dictionary = dictionary;
        _stack = stack;
        _user = user;
    }

    public IActionResult Index(string? searchStr)
    {   
        ViewData["searchStr"] = searchStr;

        _stack.Searched(searchStr);
        
        Word? result = _dictionary.LookUp(searchStr);

        return View(new IndexVM {
            Word = result,
            WordIsSaved = _user.IsSaved(result),
            WordCategories = result == null ? null : result.Categories,
            Categories = _user.GetCategories(),
            ForwardDisabled = _stack.ForwardDisabled(),
            BackwardDisabled = _stack.BackwardDisabled()
        });
    }

    public IActionResult Backward(string? currentKey)
    {
        return RedirectToAction(nameof(Index), new { searchStr = _stack.Backwarded(currentKey) });
    }
    public IActionResult Forward(string? currentKey)
    {
        return RedirectToAction(nameof(Index), new { searchStr = _stack.Forwarded(currentKey) });
    }
    public IActionResult RemoveWord(string key, string category)
    {
        Word? word = _dictionary.LookUp(key);
        if (word != null)
            _user.RemoveWord(word, category);
        return RedirectToAction(nameof(Index), new { searchStr = key });
    }
    public IActionResult Library(string? category)
    {
        if (!String.IsNullOrEmpty(category))
            return View(new SavedWordsVM {
                Category = _user.Category(category),
                Categories = _user.GetCategories().Select(c => c.Name)
            });

        return View(
            new SavedWordsVM {
                Categories = _user.GetCategories().Select(c => c.Name)
            }
        );
    }
    public IActionResult SaveWord(string key, string? category)
    {
        Word? word = _dictionary.LookUp(key);
        if (word == null) 
            return RedirectToAction(nameof(Index));
            
        if (!String.IsNullOrEmpty(category))
            _user.SaveWord(word, category.ToLower());
        return RedirectToAction(nameof(Index), new { searchStr = key });
    }
    public IActionResult Suggest(string query)
    {
        return Ok(_dictionary.Suggest(query));
    }
    public IActionResult SuggestionPartial(string model)
    {
        return PartialView("_Suggestion", model);
    }

    public IActionResult WordJumble(string category) {
        return Ok(_user.PlayWordJumble(category));
    }
}
