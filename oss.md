---
title: "Open Source"
weight: 4
---

A list of open source contributions I've made; some are small, but it's nice to
be able to remember them:

<ul class="oss_contributions">
  {% for entry in site.oss_contributions %}
    <li class="oss_contribution">
      <span class='block_labels'>
          {% for language in entry.languages | sort %}
            <span class="block_label">
                {{ language }}
            </span>
          {% endfor %}
      </span>
      {{entry.content}}
    </li>
  {% endfor %}
</ul>
