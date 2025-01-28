## Blog

I very occasionally blog about technical things, take a look:

<ul class="entries">
  {% for post in site.posts %}
    <li class="entry">
    <span class="entry_date">
    {% assign d = post.date | date: "%-d" %}
    {% case d %}{% when "1" or "21" or "31" %}{{ d }}st{% when "2" or "22" %}{{ d }}nd{% when "3" or "23" %}{{ d }}rd{% else %}{{ d }}th{% endcase %}
    {{ post.date | date: "%B" }}
    {{ post.date | date: "%Y" }}:
    </span>
    <span class="entry_link"><a href="{{ site.url }}{{ post.url }}">{{ post.title }}</a></span>
    </li>
  {% endfor %}
</ul>
