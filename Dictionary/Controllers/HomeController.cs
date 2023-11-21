using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using Dictionary.Models;
using Dictionary.Services;
using Dictionary.Models.ViewModels;
using Microsoft.AspNetCore.Http;
using Dictionary.Constants;

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
        return View(
            new SavedWordsVM {
                Category = String.IsNullOrEmpty(category) ? null : _user.Category(category),
                Categories = _user.GetCategories().Select(c => c.Name).ToList()
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

    [HttpPost]
    public IActionResult WordJumble(string? category, int i = 0) {
        if (i == 0) {
            HttpContext.Session.SetInt32(Constant.SessionKeyName.NoCorrectAnswers, 0);
            HttpContext.Session.SetInt32(Constant.SessionKeyName.NoQuestions, 0);
        }
        int? noCorrectAnswers = GetNumInSession(Constant.SessionKeyName.NoCorrectAnswers);
        int? noQuestions = GetNumInSession(Constant.SessionKeyName.NoQuestions);
        if (noCorrectAnswers == null)
            HttpContext.Session.SetInt32(Constant.SessionKeyName.NoCorrectAnswers, 0);
        if (noCorrectAnswers == null)
            HttpContext.Session.SetInt32(Constant.SessionKeyName.NoQuestions, 0);

        JumbleWord? jumbleWord = _user.PlayWordJumble(category, i);

        if (jumbleWord == null)
            return RedirectToAction(nameof(Library));

        return View(new WordJumbleVM(i, jumbleWord));
    }

    [HttpPost]
    public IActionResult WordJumblePost(string originalWord, string userAnswer, int currentIndex) {
        bool isCorrect = String.Equals(originalWord, userAnswer, StringComparison.OrdinalIgnoreCase);
        int noCorrectAnswers = GetNumInSession(Constant.SessionKeyName.NoCorrectAnswers);
        int noQuestions = GetNumInSession(Constant.SessionKeyName.NoQuestions);
        if (isCorrect) {
            HttpContext.Session.SetInt32(Constant.SessionKeyName.NoCorrectAnswers, noCorrectAnswers + 1);
        }
        HttpContext.Session.SetInt32(Constant.SessionKeyName.NoQuestions, noQuestions + 1);
        return PartialView("_JWResult", new WJResultVM(isCorrect, originalWord, currentIndex + 1));
    }
    private int GetNumInSession(string name) {
        return HttpContext.Session.GetInt32(name) ?? 0;
    }
}
