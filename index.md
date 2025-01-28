---
title: "Index"
layout: page
---

<div class="intro">
  <img src="assets/images/owenstephens.png" alt="Owen Stephens"/>

  <div>
  {% capture content %}
  {% include intro.md %}
  {% endcapture %}

  {{ content | markdownify }}
  </div>
</div>

<section>
{% capture content %}
{% include blog.md %}
{% endcapture %}

{{ content | markdownify }}
</section>

<section>
{% capture content %}
{% include oss.md %}
{% endcapture %}

{{ content | markdownify }}
</section>

<section>
{% capture content %}
{% include research.md %}
{% endcapture %}

{{ content | markdownify }}
</section>
