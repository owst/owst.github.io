require "nokogiri"

Jekyll::Hooks.register :documents, :post_render do |doc|
  next unless doc.output_ext == ".html"

  html = Nokogiri::HTML(doc.output)

  next unless (post_contents = html.at_css("#post-contents"))

  header_elements_search = (1..6).map { |i| "h#{i}" }.join(",")

  post_contents.search(header_elements_search).each do |h|
    h.add_child(%(<a class="header_link" href="##{h["id"]}">🔗</a>))
  end

  doc.output = html.to_s
end
