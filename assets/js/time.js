var Time = {
    bracketed_time_from_now: function(time) {
        return '(' + moment(time).fromNow() + ')';
    }
};

$(function() {
    var page_dates = document.getElementsByClassName("page-date");

    _.each(page_dates, function(date) {
        if (date.hasAttribute("datetime")) {
            date.innerHTML += Time.bracketed_time_from_now(date.getAttribute("datetime"));
        }
    });

});
