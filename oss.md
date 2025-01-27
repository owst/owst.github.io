## Open Source

A list of open source contributions I've made; some are small, but it's nice to
remember them:

<ul class="oss_contributions">
    {% assign entries = site.oss_contributions | sort: "weight", 'last'  %}
    {% for entry in entries %}
    <li class="oss_contribution">
      <span class='block_labels'>
          {% assign sorted_languages = entry.languages | sort %}
          {% for language in sorted_languages %}
            <span class="block_label">
                {{ language }}
            </span>
          {% endfor %}
      </span>
      {{entry.content}}
    </li>
  {% endfor %}
</ul>
