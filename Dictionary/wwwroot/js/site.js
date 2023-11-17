$(document).ready(function () {
  $("#searchInput").on("input", function () {
    var query = $(this).val();

    $.ajax({
      url: "/Home/Suggest",
      type: "GET",
      data: { query: query },
      success: function (data) {
        displaySuggestions(data);
      },
      error: function (error) {
        console.error("Error fetching suggestions: ", error);
      },
    });
  });

  
  function displaySuggestions(suggestions) {
    var suggestionList = $("#suggestions");
    suggestionList.empty();

    for (var i = 0; i < Math.min(suggestions.length, 6); i++) {
      $.get(
        "/Home/SuggestionPartial",
        { model: suggestions[i] },
        function (partialHtml) {
          suggestionList.append(partialHtml);
        }
      );
    }
  }
  
  let wjGameForm = document.getElementById("wjGame");
  wjGameForm.addEventListener("submit", (e) => {
    e.preventDefault();
    var originalWord = $("#originalWord").val();
    var userAnswer = $("#userAnswer").val();
    var currentIndex = $("#currentIndex").val();
    console.log(originalWord);
    console.log(userAnswer);
    $.ajax({
      url: "/Home/WordJumblePost",
      type: "POST",
      data: { originalWord : originalWord, userAnswer : userAnswer, currentIndex : currentIndex },
      success: function (partialHtml) {
        $("#WJResultModal .modal-body").html(partialHtml);
        $("#WJResultModal").modal("show");
      },
      error: function (err) {
        console.error("Error word jumble playing: ", err);
      }
    })
  })
});
