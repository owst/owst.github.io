<ul>
  {% assign name = {{page.title | downcase}} %}
  {% for post in site.categories[{{name}}] %}
    <li>
      <time datetime='{{ post.date | date: "%Y-%m-%d" }}'>
        {{post.date | date: "%b %d %Y"}}:
      </time> <a href="{{ site.url }}{{ post.url }}">{{ post.title }}</a>
      {% if post.sub_title %}
        <div class="entry-description">
        {{post.sub_title}}
        </div>
      {% endif %}
    </li>
  {% endfor %}
</ul>
