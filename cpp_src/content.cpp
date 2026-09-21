#include "content.h"
#include "utils.h"
#include <iostream>

// Builds content from its common fields.
Content::Content(int contentId, const std::string &contentTitle, const std::string &contentDate, int contentAuthorId)
    : id(contentId), title(contentTitle), date(contentDate), authorId(contentAuthorId) {}

// Enables safe deletion through a Content pointer.
Content::~Content() {}

// Returns the content ID.
int Content::getId() const { return id; }
// Returns the content title.
std::string Content::getTitle() const { return title; }
// Returns the content date.
std::string Content::getDate() const { return date; }
// Returns the author or organizer ID.
int Content::getAuthorId() const { return authorId; }

// Validates and changes the title.
bool Content::setTitle(const std::string &value)
{
    if (value.empty() || value.size() > 99U || value.find('|') != std::string::npos) return false;
    title = value;
    return true;
}

// Validates and changes the date.
bool Content::setDate(const std::string &value)
{
    if (!date_is_valid(value.c_str())) return false;
    date = value;
    return true;
}

// Builds a post from individual values.
Post::Post(int postId, const std::string &postTitle, const std::string &postBody, int postCategory, int postAuthorId, const std::string &postDate)
    : Content(postId, postTitle, postDate, postAuthorId), body(postBody), category(postCategory) {}

// Builds a post from a C record.
Post::Post(const PostRec &record)
    : Content(record.id, record.title, record.date, record.author_id), body(record.content), category(1) {
    if (std::string(record.category) == "2" || std::string(record.category) == "Activity") category = 2;
    else if (std::string(record.category) == "3" || std::string(record.category) == "Update") category = 3;
}

// Converts this object to a C record.
PostRec Post::toRec() const
{
    PostRec record = {};
    record.id = id; record.author_id = authorId;
    utils::copyToArray(record.title, sizeof(record.title), title);
    utils::copyToArray(record.content, sizeof(record.content), body);
    utils::copyToArray(record.category, sizeof(record.category), categoryName());
    utils::copyToArray(record.date, sizeof(record.date), date);
    return record;
}

// Returns a readable category label.
std::string Post::categoryName() const
{
    if (category == 2) return "Activity";
    if (category == 3) return "Update";
    return "Notice";
}

// Returns the post body.
std::string Post::getBody() const { return body; }

// Displays a post card.
void Post::display(const std::string &authorName) const
{
    std::cout << "\n[POST " << id << "] " << title << "\nDate: " << date << " | Category: " << categoryName() << "\nAuthor: " << authorName << "\n" << body << "\n";
}

// Returns "Post".
std::string Post::typeName() const { return "Post"; }

// Builds an event from individual values.
Event::Event(int eventId, const std::string &eventTitle, const std::string &eventDescription, const std::string &eventVenue, const std::string &eventDate, int organizerId)
    : Content(eventId, eventTitle, eventDate, organizerId), description(eventDescription), venue(eventVenue) {}

// Builds an event from a C record.
Event::Event(const EventRec &record)
    : Content(record.id, record.name, record.date, record.organizer_id), description(record.description), venue(record.venue) {}

// Converts this object to a C record.
EventRec Event::toRec() const
{
    EventRec record = {};
    record.id = id; record.organizer_id = authorId;
    utils::copyToArray(record.name, sizeof(record.name), title);
    utils::copyToArray(record.venue, sizeof(record.venue), venue);
    utils::copyToArray(record.date, sizeof(record.date), date);
    utils::copyToArray(record.description, sizeof(record.description), description);
    return record;
}

// Returns the event description.
std::string Event::getDescription() const { return description; }
// Returns the event venue.
std::string Event::getVenue() const { return venue; }

// Displays an event card.
void Event::display(const std::string &authorName) const
{
    std::cout << "\n[EVENT " << id << "] " << title << "\nDate: " << date << " | Venue: " << venue << "\nOrganizer: " << authorName << "\n" << description << "\n";
}

// Returns "Event".
std::string Event::typeName() const { return "Event"; }
