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
    public readonly IStackService _stack;
    public readonly IUserService _user;
    public HomeController(IDictionaryService dictionary, IStackService stack, IUserService user)
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
    public IActionResult RemoveWord(string key, string tag)
    {
        _user.RemoveWord(key, tag);
        return RedirectToAction(nameof(Index), new { searchStr = key });
    }
    public IActionResult ViewSavedWords(string? category)
    {
        if (!String.IsNullOrEmpty(category))
            return View(_user.GetWords(category));

        return View(new LinkedList());
    }
    public IActionResult SaveWord(string key, string? category)
    {
        if (String.IsNullOrEmpty(category))
            category = "Default";
        _user.SaveWord(key, category);
        return RedirectToAction(nameof(Index), new { searchStr = key });
    }
}
