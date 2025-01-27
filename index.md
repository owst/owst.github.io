---
title: "Index"
layout: page
---

<div class="intro">
  <img src="assets/images/owenstephens.png" alt="Owen Stephens"/>

  <div>
  {% capture content %}
  {% include_relative intro.md %}
  {% endcapture %}

  {{ content | markdownify }}
  </div>
</div>

<section>
{% capture content %}
{% include_relative blog.md %}
{% endcapture %}

{{ content | markdownify }}
</section>

<section>
{% capture content %}
{% include_relative oss.md %}
{% endcapture %}

{{ content | markdownify }}
</section>

<section>
{% capture content %}
{% include_relative research.md %}
{% endcapture %}

{{ content | markdownify }}
</section>
