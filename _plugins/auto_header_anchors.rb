require "nokogiri"

def add_header_anchors(doc)
  return unless doc.output_ext == ".html"

  html = Nokogiri::HTML(doc.output)

  return unless (container = html.at_css(".container"))

  header_elements_search = (1..6).map { |i| "h#{i}" }.join(",")

  container.search(header_elements_search).each do |h|
    h.add_child(%(<a class="header_link" href="##{h["id"]}">🔗</a>))
  end

  doc.output = html.to_s
end

Jekyll::Hooks.register :pages, :post_render do |doc|
  add_header_anchors(doc)
end

Jekyll::Hooks.register :documents, :post_render do |doc|
  add_header_anchors(doc)
end
