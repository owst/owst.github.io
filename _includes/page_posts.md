<ul class="entries">
  {% assign name = {{page.title | downcase}} %}
  {% for post in site.categories[{{name}}] %}
    <li class="entry">
      <time datetime='{{ post.date | date: "%Y-%m-%d" }}'>
        {{post.date | date: "%b %d %Y"}}:
      </time> <a href="{{ site.url }}{{ post.url }}">{{ post.title }}</a>
    </li>
  {% endfor %}
</ul>
